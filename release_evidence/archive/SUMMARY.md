# Archive index of the pages the paper cites, and the human-baseline refetch

This directory lists the third-party pages the paper relies on by index; it does not include their bodies. Each index row names one archived file: `id` (the file name, relative to the directory of the index), `title`, `url`, `retrieved_utc`, `bytes` and `sha256`. STARGA keeps the bodies privately (pages, API responses, response headers, opened source files, excerpts and command logs) and makes them available on request. A copy checks against its index row by byte count and SHA-256. The digests of the files in this directory are in `SHA256SUMS`:

```
cd release_evidence/archive && sha256sum -c SHA256SUMS
```

Retrieval date: 2026-11-06 (UTC). Leaderboard and scorecard retrieval starts at `2026-11-06T23:08:59Z` (`retrieval_utc.txt`), the human-baseline refetch is at `2026-11-06T23:09:53Z`, and the prior-art search runs from `2026-11-06T23:13:49Z` (`release_evidence/archive/prior_art/retrieval_utc.txt`) to `2026-11-06T23:29:38Z`. One leaderboard copy is the page as read on 2026-11-04.

## Indexes

| index | lists | rows |
|---|---|---:|
| `release_evidence/archive/leaderboard/INDEX.tsv` | the community leaderboard page (2026-11-04 read and 2026-11-06 retrieval) with its response headers, and four leaderboard API probes with their headers | 11 |
| `release_evidence/archive/scorecards/INDEX.tsv` | the 14 cited scorecards, each as HTML page and as API JSON, with the response headers of each | 56 |
| `release_evidence/archive/prior_art/WITHHELD.tsv` | digest and byte count of the four prior-art lists removed from this release | 4 |

Column rules:

- `url` of a curl fetch is the requested URL from the command log. `url` of a GitHub API call is the request URL built from the logged command, with the query parameters form-encoded in key order. `not recorded` marks a file with no single URL (command logs and length notes).
- `retrieved_utc` is the timestamp of the fetch from its `.utc` record or from the command log. A value written as start and end is the logged interval of the step that holds the fetch. A batch of fetches headed by one timestamp in the log carries that timestamp. `not recorded` marks a file without a logged time.
- The digest is the digest of the stored form. Stored response headers carry no load-balancer `Set-Cookie` lines, and stored GitHub API bodies carry no terminal color codes. A refetch of a live page can return other bytes.

The command log of the leaderboard, scorecard and human-baseline fetches is kept privately as well:

| file | bytes | SHA-256 |
|---|---:|---|
| `COMMANDS.log` | 28446 | `1d4b105693920ccfe384fe914bacd7caad1310d20960589c2e3c9f4044ccf1eb` |

Derived tables in this directory hold identifiers, numbers and verdict codes only:

- `release_evidence/archive/leaderboard/INDEX.tsv` also lists `rows_2026-11-04.json` and `rows_live.json`, the row objects embedded in the two leaderboard pages (name, benchmark, set, score, cost, date, scorecard id, code and paper URLs); they are kept with the page bodies. Those two files are not included in this release.
- `release_evidence/archive/leaderboard/row_diff.txt`: the comparison of those rows.
- `release_evidence/archive/prior_art/WITHHELD.tsv`: digest and byte count of each prior-art list removed from this release. Those lists name repositories the paper does not print.
- `release_evidence/archive/prior_art/SHA256SUMS`: the digests of the files under `release_evidence/archive/prior_art/`.

Sixteen code-search items are withheld. The paper does not print their repositories or their per-level counts, so those rows are not in this release. One withheld item carries per-level action counts of another play. Those counts are not printed here. The other six opened items list no per-level minima.

## 1. Community leaderboard and the 14 scorecards

The paper reads `https://arcprize.org/leaderboard/community` (citation `community`, read 2026-11-04) and the fourteen scorecards linked from that page.

| item | URL | HTTP | bytes | SHA-256 |
|---|---|---:|---:|---|
| leaderboard, read 2026-11-04 | `https://arcprize.org/leaderboard/community` | not recorded | 94226 | `5323be99e69f83ef4eef9699a7b2edb6920ceddf49c5edcc470a8d9e193cc9a8` |
| leaderboard, 2026-11-06 retrieval | same | 200 | 95683 | `9a48553557b4bf996cee8203828ab860c2b6c98632369ec2b3011419768fb206` |

The 2026-11-06 HTML is not byte-identical to the 2026-11-04 file. The row fields `name`, `score`, `cost` and `date` of the 14 cited scorecards are equal on both pages (`release_evidence/archive/leaderboard/row_diff.txt`: `cited_row_fields_name_score_cost_date=UNCHANGED`, 14/14 present on both pages). The page has no separate JSON API: four probes returned HTTP 404 (`/api/leaderboard/community`, `/api/v3/leaderboard`, `/api/v3/leaderboard/community`, `/leaderboard/community.json`). The structured rows are the objects embedded in the HTML, extracted to the files `rows_2026-11-04.json` and `rows_live.json` of the folder `release_evidence/archive/leaderboard/`. Those two files are not included in this release.

Each cited scorecard is archived as HTML and as `GET https://arcprize.org/api/v3/scorecards/<id>` JSON. All 14 API calls returned HTTP 200 and parse as JSON with `card_id` equal to the requested id (`scorecard_api_json_ok=14/14`). The HTML document is the Next.js shell for `/scorecards/[card_id]` (title `ARC Prize`); the scorecard body is the API JSON. Checked totals from that JSON: Tycho score 100, 6641 actions, 183 levels; baseline1 score 98.97, 8347 actions, 183 levels; Retrodict score 99.86, 7703 actions, 183 levels.

## 2. Human baselines

The files under `release_evidence/archive/human_baselines/` stay in this directory: the two API responses with their headers and timestamps, the endpoint record and the row comparison.

`release_evidence/human_baselines.json` has no per-level rows. It names column `human_baseline` of `data/demo25_levels_mmae.csv`, pins SHA-256 `9be40c55cccb0e39f3a884d42f9284c717999fe4eb2f710313b6a812d182be64`, and sets `retrieval_date` to `not recorded in the source table`. That hash is the SHA-256 of the 183 column values, one per line in table order, each newline-terminated.

Refetch:

- endpoint: `GET https://three.arcprize.org/api/games`. A check on 2026-11-20 without a key returned HTTP 401. The 2026-11-06 call used a key and returned HTTP 200.
- header: `X-Api-Key` (value not stored), `Accept: application/json`
- retrieval UTC: `2026-11-06T23:09:53Z`
- HTTP 200, 2741 bytes
- response: a list of 25 objects
- field names: `baseline_actions`, `game_id`, `tags`, `title`
- `baseline_actions` is the per-level human baseline; index 0 is level 1

A second call, `GET https://three.arcprize.org/api/games/sb26` at `2026-11-06T23:09:54Z`, returned the same list for that game plus `default_fps`. A check on 2026-11-20 returned HTTP 404. Last reached 2026-11-06. `game_id` is `sb26-7fbdac44`. The eight values are 18, 28, 18, 19, 31, 23, 58, 18.

Row comparison against the CSV column named by `release_evidence/human_baselines.json` (`release_evidence/archive/human_baselines/comparison.tsv`): 183 rows, 183 matches, 0 mismatches, both sums 17135. The same 25 `game_id` values and the same counts are in the scorecard kit's local `metadata.json` files (`kit_vs_api_value_mismatches=0`, `kit_vs_api_game_id_differences=0`). Those `metadata.json` files are not included in this release.

## 3. Prior-art search

Search date 2026-11-06. First query at `2026-11-06T23:13:49Z`. GitHub code search is `GET /search/code` with `per_page=10`. Every response has `incomplete_results=false`. The list of stored items is withheld (`release_evidence/archive/prior_art/WITHHELD.tsv`). One returned Q10 item lies in a repository that is not public; it holds no per-level list and is not stored. The stored response holds 9 of the 10 returned items. That response is not included in this release.

| id | UTC | query | total_count | items returned |
|---|---|---|---:|---:|
| Q01 | 2026-11-06T23:13:49Z | `ARC-AGI-3 optimal actions` | 1542 | 10 |
| Q02 | 2026-11-06T23:14:00Z | `"ARC-AGI-3" "minimum actions"` | 8 | 8 |
| Q03 | 2026-11-06T23:14:09Z | `"ARC-AGI-3" "per-level" optimal` | 261 | 10 |
| Q04 | 2026-11-06T23:14:18Z | `"ARC-AGI-3" shortest solution` | 381 | 10 |
| Q05 | 2026-11-06T23:14:27Z | `"baseline_actions" optimal` | 123 | 10 |
| Q06 | 2026-11-06T23:14:38Z | `org:arcprize optimal actions` | 1 | 1 |
| Q07 | 2026-11-06T23:14:46Z | `"ARC-AGI-3" "proven minimum"` | 0 | 0 |
| Q08 | 2026-11-06T23:14:54Z | `"optimal playthrough" ARC-AGI-3` | 6 | 6 |
| Q09 | 2026-11-06T23:15:03Z | `"fewest actions" ARC-AGI-3` | 49 | 10 |
| Q10 | 2026-11-06T23:15:12Z | `"ARC-AGI-3" solver minimum level` | 547 | 10 |
| Q11 | 2026-11-06T23:15:21Z | `"arc-agi-3" min_actions` | 55 | 10 |
| Q12 | 2026-11-06T23:15:31Z | `ARC-AGI-3 "per level" "actions" optimal OR minimum OR shortest` | 9 | 9 |

Opened in full: every returned item of Q02, Q06, Q07, Q08, and Q12. From the other queries, the returned files whose names point at a solver, a search, a result table, or a baseline. The repository names and the per-level counts taken from those files are withheld. The paper does not print them. `release_evidence/archive/prior_art/WITHHELD.tsv` identifies the removed lists by digest.

The ARC-AGI-3 documentation says the fewest actions per level is recorded as the baseline. That sentence defines the human baseline. Six hits are copies of the ARC-AGI-3 paper. That paper defines the best first-run human playthrough as the fewest actions any participant used on each level, and it does not list the 183 counts. One agent template reads the human baseline from the environment and does not hardcode a length table.

No opened file lists a per-level minimum for all 183 public levels. The query `"ARC-AGI-3" "proven minimum"` returned `total_count` 0.

One authenticated code-search hit is one non-public file. The opened file contains no per-level length list. Its body is not stored and has no index row.

### Forum and community chat

- GitHub issues search `ARC-AGI-3 optimal OR minimum actions per level`: rc=0, `total_count` 540. The returned page is ordinary issues, not a table of per-level lengths.
- `GET repos/arcprize/ARC-AGI/discussions?per_page=10`: rc=1. Body message: discussions are disabled for this repository.
- `https://forum.arcprize.org`: curl rc=6, `http=000`, name not resolved. Checked 2026-11-20: the name still does not resolve. The 2026-11-06 record has no later success.
- `https://community.arcprize.org`: curl rc=6, `http=000`, name not resolved. Checked 2026-11-20: the name still does not resolve. The 2026-11-06 record has no later success.
- `https://arcprize.org/forum`: rc=0, `http=404`, 32090 bytes. Checked 2026-11-20: HTTP 404.
- Community chat invite (ARC Prize server): rc=0, `http=200`, metadata only, no messages.
- DuckDuckGo HTML query `ARC-AGI-3 minimum actions per level solver`: rc=0, `http=200`, 10 result links. One link is a third-party repository in the withheld records. One link is the human playthrough page `https://arcprize.org/arc-agi/3/leaderboard?task=ls20`.
- Community chat search `ARC-AGI-3 optimal actions per level`: rc=0, `http=202`, 0 result links.
- `https://www.answeroverflow.com/search?q=ARC-AGI-3+optimal+minimum+actions`: rc=0, `http=429`, 0 result links. Checked 2026-11-20: HTTP 429.

The community chat message archive is not read. No bot login is used.
