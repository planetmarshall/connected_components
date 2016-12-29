#include <gtest/gtest.h>
#include <Magick++.h>

int main(int argc, char **argv) {
	Magick::InitializeMagick(*argv);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
