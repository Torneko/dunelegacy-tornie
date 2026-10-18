#include <catch2/catch_all.hpp>
#include <misc/VideoModes.h>

TEST_CASE("A single desktop mode still offers smaller window resolutions", "[video]") {
    const auto choices = VideoModes::choices(Coord(1920,1080), {Coord(1920,1080)}, Coord(1280,720));
    for(const Coord size : {Coord(640,480), Coord(800,600), Coord(1280,720), Coord(1600,900), Coord(1920,1080)})
        REQUIRE(std::find(choices.begin(), choices.end(), size) != choices.end());
    REQUIRE(std::count(choices.begin(), choices.end(), Coord(1920,1080)) == 1);
}

TEST_CASE("A stale two-monitor setting is bounded by the selected monitor", "[video]") {
    const Coord monitor(1920,1080), combined(3840,1080);
    REQUIRE(VideoModes::fit(combined, monitor) == monitor);
    REQUIRE(VideoModes::fit(Coord(1280,720), combined) == Coord(1280,720));
    const auto choices = VideoModes::choices(monitor, {combined, monitor, monitor}, combined);
    REQUIRE(std::find(choices.begin(), choices.end(), combined) == choices.end());
    REQUIRE(std::find(choices.begin(), choices.end(), monitor) != choices.end());
}

TEST_CASE("Ultrawide monitors and valid custom sizes are preserved", "[video]") {
    const Coord ultrawide(3440,1440), custom(1100,700);
    const auto choices = VideoModes::choices(ultrawide, {}, custom);
    REQUIRE(VideoModes::fit(ultrawide, ultrawide) == ultrawide);
    REQUIRE(std::find(choices.begin(), choices.end(), ultrawide) != choices.end());
    REQUIRE(std::find(choices.begin(), choices.end(), custom) != choices.end());
    const auto fallback = VideoModes::choices(Coord(0,0), {}, Coord(-1,0));
    REQUIRE(fallback == std::vector<Coord>{Coord(640,480)});
}
