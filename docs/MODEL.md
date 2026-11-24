# NAES-T1: network and file formats

## Network

| Part | Shape | Operation |
|---|---|---|
| Input | 49 x 64 x 64 | current frame one-hot (16), previous frame one-hot (16), own previous action one-hot (8, broadcast; 0 = none or RESET, 1 to 7 = ACTION1 to ACTION7), own previous click cell (1 plane: a single 1.0, or all zeros), repeat count min(run, 7) one-hot (8, broadcast; run = how many times in a row the same action and click cell repeated) |
| `inp` | 49 -> 64, 3 x 3, padding 1 | ternary conv, ReLU |
| `blocks.0` .. `blocks.7` | 64 -> 64, 3 x 3, dilation 1, 2, 4, 8, 16, 1, 2, 4 (padding = dilation) | h = ReLU(conv(h)) + h |
| `act_head` | 128 -> 7, 1 x 1 | input = concat(mean over 64 x 64, max over 64 x 64); action = argmax + 1 (ACTION1 .. ACTION7) |
| `click_head` | 64 -> 1, 1 x 1 | used only when action = 6; cell = argmax over 4,096 cells, cell = y * 64 + x |

Every convolution weight is `tern * scale[out]`, with `tern` in {-1, 0, +1} and one f32 scale and one f32 bias per output channel: 324,096 ternary values (194,444 non-zero) and 1,168 scales and biases.

Work per decision: 794,112,517 integer additions (about 0.79 billion). The rule: non-zero weights x output positions, stem + 8 blocks + action head; the click head runs only on a click.

## Grouped-sum kernel

The native x86-64 build decides with the grouped-sum kernel (the `naes_t1_fast` files in `src/mind/`). The reference walk (`naes_t1.mind`) is the definition of the network; the kernel computes the same sums with a different grouping. It is exact integer arithmetic with no float, so it gives the same decisions. `verify` and `bench` compare every decision with the fixture.

How a block is computed:

1. **Plans from the weights, at start.** For each of the 8 blocks the kernel reads the ternary weights of the 64 input channels in groups of 3. For one output channel, one kernel row and one horizontal tap, the three weights of a group are one code (the base-3 value of the three weights, from -13 to 13). Code 0 costs nothing. A code and its negative share one bank row: the sign says whether that row is added to the sum or taken from it. The groups are split into segments of 8. For each output channel, kernel row and segment, the plan lists the bank rows to add and the bank rows to subtract.
2. **Group bank.** For one source row of the block input, the bank holds, for every group of 3 channels and every code from 1 to 13, the row of sums of the three channel rows with the weights of that code. The rows are padded, so a horizontal tap is a shift into the same row. One bank serves all 64 output channels.
3. **Row walk.** For each source row and segment, the walk adds and subtracts the listed bank rows into 64-bit accumulator rows of the output channels the kernel row reaches, in tiles of 32 rows. It uses 256-bit integer vector instructions (AVX2), so the native executable needs a CPU with AVX2.
4. **Epilogue.** When the sums of a row are complete, the kernel scales, rounds and clamps them with the integer multiplier and shift of the layer, the same fixed-point rule as the reference walk. The stem is a table of the input planes, and the heads (mean and max pooling, the 7 action logits, the click cells) use vector sums with the same tie rule: the smallest index wins.

The configuration is fixed in the program: groups of 3 channels, 8 groups for each segment, tiles of 32 rows, one thread in each worker process. No option changes it; `bench --threads` changes only how many worker processes run. `verify`, `serve` without `--trace`, `play` and `bench` use the kernel. `selftest`, `serve --trace` and every mode of the MLIR unit use the reference walk.

## Bench output (`naes_t1 bench`)

```
naes_t1 bench --weights FILE --fixture FILE [--threads 1..24] [--batch 1|25] [--offset N] [--limit N]
```

The mode is in the native x86-64 build only; the MLIR unit prints that bench needs the x86-64 native build and exits with status 2. It builds the plans from the weights, then decides the fixture contexts in timed windows (`--batch` decisions in a window, the windows start at record `--offset` and go around the 5,499 records; `--limit` is the number of windows, at most 5,499 divided by the batch). Then it decides all 5,499 contexts again without a clock and compares every answer with the fixture. `--threads N` (1 to 24) runs the smaller of N and the CPUs the process may use as worker processes, each on one CPU; with 2 or more it uses the multi-worker walk below, which has no windows: `--batch` above 1, `--offset` and `--limit` are a usage error with it.

| Stream | Line | Meaning |
|---|---|---|
| standard output | `PLAN g cg tile terms adds bytes` | group size, groups for each segment, tile rows, then the count of bank terms, the count of additions and the bank size for each segment |
| standard output | `COMPARED=5499` | the number of decisions compared |
| standard output | `MATCH k/5499` | how many decisions equal the fixture |
| standard output | `STREAM_SHA256` | SHA-256 of the 16,497-byte decision stream; the same value as the `verify --records` digest |
| standard output | `SERVE_SHA256` | SHA-256 of the 5,499 serve requests (8,196 bytes each) that the bench builds from the fixture |
| standard error | `WARMUP_CALLS n` | one worker: the number of untimed decisions before the timed windows |
| standard error | `SAMPLE k first ns bad` | one worker: one line for each timed window: its number, its first record, its time in nanoseconds and its wrong decisions |
| standard error | `MISMATCHES_TIMED n` | wrong decisions inside the timed part (the timed windows, or the timed walk of 2 or more workers); 0 on a pass |
| standard error | `US_PER_DECISION n` | microseconds per decision over the timed part: the wall time of the timed part divided by its decisions |
| standard error | `WORKER_PROCESSES n` | 2 or more workers: the number of worker processes that ran |
| standard error | `WORKER_DECISIONS a b ...` | 2 or more workers: the decisions of each worker, in worker order |
| standard error | `WORKER_FAILED n`, `NOT_COVERED n` | 2 or more workers, on a failure only: the number of failed workers that the main process saw when it stopped the run, or the records that were not decided exactly once |
| standard error | `PHASE_US a b c` | one worker: microseconds per decision in the stem, the blocks and the heads |
| standard error | `BLOCK_US a b c` | one worker: microseconds per decision in the blocks, split into the group banks, the row walk and the epilogue |

Exit status, the same list for every worker count: 0 full match; 1 a wrong decision; 2 usage error (including `--threads` outside 1 to 24, and `--batch` above 1, `--offset` or `--limit` with 2 or more workers); 3 weights rejected; 4 fixture rejected or no memory; 5 activation guard tripped; 6 digest failure; 7 the stream digest differs from the recorded value; 8 a worker process failed or a record was not decided exactly once (2 or more workers only).

### Multi-worker walk

With `--threads 2` or more the bench starts worker processes instead of timing one. The main process builds the plans and the serve block, then starts one worker process for each CPU it uses, and each worker runs on one CPU. The replies go into one shared block, one 3-byte slot for each record, in record order, so the stream does not depend on which worker decided a record. The main process sends the record numbers to the workers through pipes, and a worker takes a number, decides that record with the same grouped-sum kernel and writes the reply into its slot. Each record is decided exactly once: the shared block holds a mark for each record, and a record with no mark, or a worker that stops with an error, ends the run with status 8. The first 1,500 records go in batches of 25, and the rest form the timed walk. When all workers are done, the main process computes `STREAM_SHA256` from the shared block, so every worker count gives the same decision stream as one worker.

The inputs are the frames, the model's own previous action, its own previous click cell and the repeat count only. There is no game id, level id, step counter or game state among them.

## Weight file: `model/naes_t1.bin`

Little-endian.

```
magic      8 bytes   "NAEST1W\0"
version    u32       1
n_layers   u32       11
repeat n_layers times, in this order: inp, blocks.0 .. blocks.7, act_head, click_head
  name_len u32
  name     name_len ASCII bytes
  out_c    u32
  in_c     u32
  kh       u32
  kw       u32
  dilation u32
  tern     out_c * in_c * kh * kw  int8, order [out][in][ky][kx]
  scale    out_c  f32
  bias     out_c  f32
```


Size 329,133 bytes. Its SHA-256 is `fa54f507fdf9a9c74a8bc22d95a1c970e3528b3fe1707b6eb61d74bad2419fec`.

The MIND loader (`src/mind/naes_t1_weights.mind`) reads a second image form as well, with the magic `NAEST1WB` (8 bytes, no zero byte at the end). It is little-endian: `version` u32 (1), `n_layers` u32 (11), then a table of `n_layers` descriptors of 16 bytes each (`out_c`, `in_c`, `k`, `dilation`, each u32; the kernel is square and the first `out_c`, the channel count, must be 64), then for each layer in the order above: the `out_c * in_c * k * k` ternary int8 values in the order [out][in][ky][kx], padding up to a multiple of 4 bytes, `out_c` f32 scales and `out_c` f32 biases. This form stores no layer names. The form of `model/naes_t1.bin` is the one above, with the magic `NAEST1W\0`.

For both forms the loader requires exactly these shapes and dilations (and, in the named form, these layer names), and rejects any ternary byte other than -1, 0 or +1, any non-finite value, a scale of magnitude 2^8 or more and a bias of magnitude 2^16 or more.

## Serve protocol (`naes_t1 serve`)

One request per move on stdin, one reply on stdout, until EOF.

```
request  8,196 bytes:
  bytes 0-4095      current frame, u8[4096], row-major 64 x 64 (cell = 64 * y + x), colours 0-15
  bytes 4096-8191   previous frame, u8[4096] (at the first decision: a copy of the settled
                    opening-RESET frame)
  byte  8192        previous action, u8 (0 = none or RESET, otherwise 1-7)
  bytes 8193-8194   previous click cell, i16 little-endian (-1 = none)
  byte  8195        repeat count of the previous action and cell, u8 (clamped at 7)
reply    3 bytes:   action u8 (1-7), x u8, y u8 (x = y = 255 unless the action is 6, a click)
```

The server exits with status 0 at a clean EOF, rejects a truncated record or an out-of-range colour, action or cell with status 4, and reports rejected weights with status 3. Each process owns its workspace and keeps no state between requests, so several can run at once (one per game).

## Fixture file: `naes_t1_fixture.bin` (included in the release)

The decision contexts of the final run, in play order, one per non-RESET action, each with the decision the run took; `naes_t1 verify` replays them.

```
magic      8 bytes   "NAEST1F\0"
count      u32       5,499
repeat count times, 8,200 bytes each:
  bytes 0-8191      current frame, then previous frame (as in a serve request)
  byte  8192        previous action, u8 (as in a serve request)
  byte  8193        repeat count of the previous action and cell, u8
  bytes 8194-8195   previous click cell, i16 little-endian (-1 = none)
  byte  8196        expected action, u8 (1-7)
  byte  8197        zero
  bytes 8198-8199   expected click cell, i16 little-endian (0-4095 for action 6, else -1)
```

The tail order differs from a serve request. The fixture stores the repeat count before the click cell (bytes 8193, 8194-8195). The serve request stores the click cell first (bytes 8193-8194, 8195).


Size is 45,091,812 bytes: a 12-byte header plus 8,200 bytes per decision. Its SHA-256 is `a2dc9485252ddd922d742f7e36191fa92a20bbdb7680a2ca1c378874ecd7b925`.
