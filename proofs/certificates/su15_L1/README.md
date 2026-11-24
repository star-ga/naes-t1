# su15 level 1: engine breadth-first search

`engine_bfs_L1.log` is a breadth-first search on the official engine from the level start. Every state at depths 0 to 6 is fully expanded and none wins; the first win is at depth 7 (7 clicks). The route replays on the engine and completes the level at action 7. The search counts every display-pixel click (children generated per depth are in the log; classes merge pixels with the same effect). The log was run on the current pinned game file.
