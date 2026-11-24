#!/usr/bin/env bash
# Build the release program with the published mindc and run the selftest.
# Usage: selftest-build.sh <out-dir>   (run from the repository root)
# Same script runs on the x86-64 runner, the arm64 runner and the local preflight.
set -euo pipefail

OUT="${1:?usage: selftest-build.sh <out-dir>}"
# The compiler release is pinned by tag and archive digest.
MINDC_TAG="v0.10.4"
SHA_X64="b887ddedaa03a5a5d7124456d4b433bce5782b66d9dacac58c9dcf9083dc8003"
SHA_ARM64="4856acf7360e196e09d7576fd3d2d4b3c2c98d344350c313a2c20fba5d7f2990"
STD_DIR_IN_ARCHIVE="std"   # directory of the standard library inside the release archive
MINDC_BASE="https://github.com/star-ga/mind/releases/download/${MINDC_TAG}"

# The marker string is built here, so the filled script holds no placeholder text.
PH="__PIN_AT""_TAG__"
for v in "$MINDC_TAG" "$SHA_X64" "$SHA_ARM64" "$STD_DIR_IN_ARCHIVE"; do
  [ "$v" != "$PH" ] || { echo "unpinned value: fill the pins at the compiler tag" >&2; exit 3; }
done
# Until the list is confirmed at the tag, its first line is the marker (a lone line, so cat fails on it).
[ "$(head -n 1 src/unit_order.txt)" != "$PH" ] || { echo "src/unit_order.txt is not confirmed at the tag" >&2; exit 3; }
[ "$(cut -d' ' -f1 src/naes_t1_unit.sha256)" != "$PH" ] || { echo "unit digest not pinned" >&2; exit 3; }

SUDO=""
if [ "$(id -u)" -ne 0 ]; then SUDO="sudo"; fi

ARCH="$(uname -m)"
case "$ARCH" in
  x86_64)  ASSET="mindc-linux-x64.tar.gz";   WANT="$SHA_X64" ;;
  aarch64) ASSET="mindc-linux-arm64.tar.gz"; WANT="$SHA_ARM64" ;;
  *) echo "unsupported machine: $ARCH" >&2; exit 2 ;;
esac

mkdir -p "$OUT"
echo "UNAME_M $ARCH" | tee "$OUT/uname.txt"

export DEBIAN_FRONTEND=noninteractive
$SUDO apt-get update -y
$SUDO apt-get install -y --no-install-recommends ca-certificates curl gnupg build-essential
if [ "$ARCH" = "aarch64" ]; then
echo "::group::LLVM 20 toolchain"
curl -fsSL https://apt.llvm.org/llvm-snapshot.gpg.key | $SUDO gpg --dearmor --yes -o /usr/share/keyrings/llvm-apt.gpg
echo "deb [signed-by=/usr/share/keyrings/llvm-apt.gpg] https://apt.llvm.org/noble/ llvm-toolchain-noble-20 main" \
  | $SUDO tee /etc/apt/sources.list.d/llvm20.list >/dev/null
$SUDO apt-get update -y
$SUDO apt-get install -y --no-install-recommends llvm-20 mlir-20-tools clang-20
export PATH="/usr/lib/llvm-20/bin:$PATH"
command -v mlir-opt mlir-translate clang
clang --version | head -1
echo "::endgroup::"
fi

echo "::group::mindc download and check"
DL="$(mktemp -d)"
curl -fsSL -o "$DL/$ASSET" "$MINDC_BASE/$ASSET"
GOT="$(sha256sum "$DL/$ASSET" | cut -d' ' -f1)"
echo "mindc archive sha256 $GOT (want $WANT)"
if [ "$GOT" != "$WANT" ]; then echo "mindc archive hash mismatch" >&2; exit 6; fi
# The archive root holds mindc and the std directory side by side.
mkdir -p "$DL/arch"
tar -xzf "$DL/$ASSET" -C "$DL/arch"
export PATH="$DL/arch:$PATH"
mindc --version
echo "::endgroup::"

echo "::group::build"
sha256sum model/naes_t1.bin
mkdir -p build
# Both units hold standard-library modules of the compiler release, so both builds link its std directory.
STD="$DL/arch/$STD_DIR_IN_ARCHIVE"
ln -sfn "$STD" std
if [ "$ARCH" = "x86_64" ]; then
  # Same lines as the README build section.
  # The unit holds the standard-library modules of the list, so their import lines are taken out.
  MODS="$(sed -n 's#^std/\(.*\)\.mind$#\1#p' src/unit_order.txt | paste -sd'|')"
  # shellcheck disable=SC2046 # the list holds paths without spaces; the README lines are the same
  cat $(cat src/unit_order.txt) | grep -vE "^import std\.(${MODS});" > build/naes_t1_unit.mind
  sha256sum -c src/naes_t1_unit.sha256
  mindc build build/naes_t1_unit.mind --no-cache --out naes_t1
else
  # The pinned unit digest covers the x86-64 unit. The aarch64 unit has no pinned digest.
  # The MLIR unit has its own list: the sources and the stub, no grouped-sum kernel.
  MODS="$(sed -n 's#^std/\(.*\)\.mind$#\1#p' src/unit_order_mlir.txt | paste -sd'|')"
  # shellcheck disable=SC2046 # same as above
  cat $(cat src/unit_order_mlir.txt) | grep -vE "^import std\.(${MODS});" > build/naes_t1_unit.mind
  mindc build build/naes_t1_unit.mind --backend mlir --no-cache --out naes_t1
fi
echo "::endgroup::"

echo "::group::selftest"
./naes_t1 selftest model/naes_t1.bin > "$OUT/selftest.first.txt"
./naes_t1 selftest model/naes_t1.bin > "$OUT/selftest.second.txt"
cmp "$OUT/selftest.first.txt" "$OUT/selftest.second.txt"
cat "$OUT/selftest.first.txt"
grep -E '^SELFTEST_SHA256 ' "$OUT/selftest.first.txt" > "$OUT/selftest_sha256.line"
cat "$OUT/selftest_sha256.line"
echo "::endgroup::"
