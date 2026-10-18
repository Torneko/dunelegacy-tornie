# Dune Legacy Unit Tests

This directory contains unit tests for Dune Legacy using the Catch2 testing framework.

## Test Suites

### PathBudgetTestCase
Tests for the PathBudget synchronization protocol used in multiplayer.
See: `.analysis/features/pathbudget-sync/design.md` for the protocol specification.

Tests cover:
- **DESYNC detection** - Verifies correct detection of budget mismatches
- **Budget-for-decision-time** - Verifies clients report correct budget values
- **Timing** - Verifies stats/decision cycle calculations

### NetworkManagerTestCase  
Tests for NetworkManager functionality:
- Packet type constants
- Address utilities (Address2String)
- ENet packet stream serialization
- Keep-alive timing constants
- NAT traversal constants

## Building Tests

Tests are optional and require Catch2. To enable:

```bash
# Install Catch2 via Homebrew (macOS)
brew install catch2

# Or via system package manager (Linux)
sudo apt-get install catch2  # Debian/Ubuntu

# Configure with tests enabled
cmake -B build -DDUNELEGACY_BUILD_TESTS=ON

# Build
cmake --build build --target dunelegacy_tests

# Run tests
./build/dunelegacy_tests
```

### Running Specific Tests

```bash
# Run only PathBudget tests
./build/dunelegacy_tests "[pathbudget]"

# Run only DESYNC tests
./build/dunelegacy_tests "[desync]"

# Run only network tests
./build/dunelegacy_tests "[network]"

# List all tests
./build/dunelegacy_tests --list-tests
```

## Test Structure

Tests use Catch2's TEST_CASE macro:

```cpp
TEST_CASE("Description", "[tags]") {
    REQUIRE(condition);
    REQUIRE_FALSE(not_condition);
}
```

Tests requiring ENet initialization use the `ENetFixture`:

```cpp
TEST_CASE_METHOD(ENetFixture, "Description", "[network]") {
    // ENet is initialized here
}
```

## Adding New Tests

1. Create a new directory: `tests/YourTestCase/`
2. Create header and cpp files
3. Add source to `tests/CMakeLists.txt`
4. Use Catch2 macros for assertions


## Internet relay regression

Build both `dunelegacy` and `dunelegacy_tests` with `DUNELEGACY_BUILD_TESTS=ON`, then run CTest. `dunelegacy_network` calls `dunelegacy --verify-network` without graphics or original game data. It uses real UDP loopback sockets, filters guest-to-guest datagrams, checks the direct path and host fallback, lobby changes, twenty command cycles with preserved sender IDs, chat, selections, stale epochs, host authority, additional admission, disconnect/reconnect and host loss. It also retains the two-player co-op transport regression. It needs permission to open loopback UDP sockets. The Linux packaging workflow runs both CTest entries before uploading packages.

`dunelegacy --verify-startup --window` initializes the full runtime and draws the actual main menu, then exits. Linux release CI runs it on both packaged AppImages in French/English with Xvfb and PulseAudio, plus a software fallback check with the dummy video backend.

`dunelegacy --verify-video` additionally opens the actual video-options menu, checks the resolution list and selection, captures it when `DUNELEGACY_SMOKE_DIR` is set, and exercises a fullscreen/window round trip. Linux package CI uses a one-mode Xvfb display (`-noreset`) in FR/EN with 1280×720 window/fullscreen and a stale 3840×1080 setting on a 1920×1080 monitor. `DUNELEGACY_EXPECT_VIDEO_SIZE` checks the applied size. SDL must be built with XRandR; its development-header set is part of the vcpkg cache key, and libXrandr is bundled explicitly.
