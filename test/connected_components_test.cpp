#include <gtest/gtest.h>
#include <exception>
#include <vector>
#include <connected_components.hpp>
#include <Magick++.h>

namespace algodynamic {
	namespace test {
		class test_image {
			int m_width;
			int m_height;
			Magick::Image m_image;
		public:
			test_image() :
				m_width(0), m_height(0) {}
			test_image( const Magick::Image & img ) : m_image(img) {}
			int width() const { return m_image.columns();  }
			int height() const { return m_image.rows();  }
		};

		TEST(connected_components_test, given_empty_image_constructor_throws_exception) {
			test_image image;
			ASSERT_THROW((connected_components<test_image>(image)), empty_image_exception);
		}

		TEST(connected_components_test, given_test_image_can_find_three_components)
		{
			Magick::Image image;
			try
			{
				image.read("test.png");
			}
			// swallow warnings
			catch ( Magick::Warning & ex)
			{
				std::cerr << ex.what() << std::endl;
			}
			catch ( Magick::Exception & ex)
			{
				FAIL() << ex.what();
			}

			test_image t(image);
			connected_components<test_image> components(t);
			ASSERT_EQ(3, components.size());
		}


	}
}

