# What the seed argument reaches

File hashes: g50t.py `6f21f2065744e484ac38534f940158034e69f39add3b10a3d9fd258de7293aa9`. The engine lines are from the installed arcengine 0.9.3 and arc-agi 0.9.9 that replayed the route.

## g50t.py, the level list is a literal

```
levels = [
    # Level 1
    Level(
        sprites=[
            sprites["awhgrxsdnu"].clone().set_position(20, 14),
```

Line 1634 starts `levels`. The sprites and positions are written in the source. The constructor below does not take a seed and does not pass one on.

## g50t.py, class G50t

```
class G50t(ARCBaseGame):
    vgwycxsxjz: qxlodtievc
    ucorwtereb: int
    twyixucrqi: Sprite
    qgzorkgosv: bool
    hctlyapjnq: bool

    def __init__(self) -> None:
        camera = Camera(background=BACKGROUND_COLOR, letter_box=PADDING_COLOR)
        super().__init__(
            game_id="g50t",
            camera=camera,
            levels=levels,
            available_actions=[1, 2, 3, 4, 5],
        )
```

Lines 2780–2794. A search of this file for `seed`, `_seed`, `random`, `Random`, and `np.random` returned no matches.

## arc_agi/local_wrapper.py, the loader drops an undeclared seed

```
        sig = inspect.signature(cls)
        if seed is not None and "seed" in sig.parameters:
            kwargs = {"seed": seed}
        else:
            kwargs = {}

        self._game = cls(**kwargs)  # type: ignore[arg-type]
```

Lines 163–169. `G50t.__init__` has no `seed` parameter, so `kwargs` stays empty.

## arcengine/base_game.py, the field is stored and never read

```
    _seed: int
```

Line 45.

```
        seed: int = 0,
```

Line 55.

```
        self._seed = seed
```

Line 96. A search of the installed arcengine package for `_seed` returns only lines 45 and 96. A search of that package for `random`, `Random`, and `np.random` returns no matches. Because `G50t` calls `super().__init__` without `seed`, the stored value is the default 0. The replay observed `stored_seed_after_init` 0 and `stored_seed_after_reset` 0 for both seed arguments 0 and 11.
