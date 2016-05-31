#pragma once

#include "mld/types.hpp"
#include <utility>
#include <vector>
#include <algorithm>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace mld {
	namespace imaging {

		struct pixel_index {
			int I;
			int J;
			pixel_index(int row, int column) : I(column), J(row) {}
			pixel_index operator+(const pixel_index & idx) {
				return pixel_index(idx.J + J, idx.I + I);
			}
			bool operator==(const pixel_index & idx) const {
				return idx.I == I && idx.J == J;
			}

			bool operator!=(const pixel_index & idx) const {
				return !(operator==(idx));
			}


		};

		template< typename PixelT, typename ComponentVisitor >
		class connected_components
		{
			typedef pixel_index Idx;
			static Idx m_directions[];
			Image<PixelT> m_image;
			Image<int> m_labels;
			ComponentVisitor & m_component_visitor;
			void trace_contour(pixel_index, int, int, bool is_external);

			std::pair<std::pair<pixel_index, int>, bool> tracer(pixel_index, int, int, bool is_external);

		public:
			connected_components(const Image<PixelT> & img, ComponentVisitor & visitor);
			Image<int> labels() { return m_labels; }
		};

		template< typename PixelT, typename ComponentVisitor >
		connected_components<PixelT, ComponentVisitor>::connected_components(const Image<PixelT> & img, ComponentVisitor & visitor) :
			m_image(img),
			m_component_visitor(visitor),
			m_labels(Image<int>::Zero(img.rows(), img.cols()))
		{
			int last_row_index = m_image.rows() - 1;
			int last_column_index = m_image.cols() - 1;
			int h = m_labels.rows();
			int w = m_labels.cols();
			int label = 1;

			m_image.row(0).fill(255);
			m_image.row(last_row_index).fill(255);

			for (int j = 0; j < h; ++j) {
				auto pRow = m_image.row(j);
				pRow[0] = pRow[last_column_index] = 255;
			}

			for (int j = 1; j < last_row_index; ++j)
			{
				auto pSrcAbove = m_image.row(j - 1);
				auto pSrc = m_image.row(j);
				auto pSrcBelow = m_image.row(j + 1);
				auto pLbl = m_labels.row(j);
				auto pLblBelow = m_labels.row(j + 1);
				for (int i = 1; i < last_column_index; ++i)
				{
					// Find first black pixel
					if (pSrc[i] == 0)
					{
						// P is unlabeled and pixel above is white
						if (pLbl[i] == 0 && pSrcAbove[i] == 255)
						{
							// new external contour of new component
							m_component_visitor.init(label);
							trace_contour(Idx(j, i), 7, label, true);

							label += 1;
						}
						// pixel below P is unmarked white pixel
						else if (pLblBelow[i] == 0 && pSrcBelow[i] == 255)
						{
							// already labeled
							int l = 0;
							if (pLbl[i] != 0)
							{
								// external and internal contour
								l = pLbl[i];
							}
							else
							{
								// point immediately to the left is labeled
								l = pLbl[i - 1];
							}
							trace_contour(Idx(j, i), 3, l, false);
						}
						else if (pLbl[i] == 0)
						{
							// not a contour point
							pLbl[i] = pLbl[i - 1];
						}
					}
				}
			}

		}

		template< typename binary_image_t, typename ComponentVisitor >
		pixel_index connected_components<binary_image_t, ComponentVisitor>::m_directions[] = {
			Idx(0, 1), Idx(1, 1), Idx(1, 0), Idx(1, -1), Idx(0, -1), Idx(-1, -1), Idx(-1, 0), Idx(-1, 1)
		};

		template<typename binary_image_t, typename ComponentVisitor >
		std::pair<std::pair<pixel_index, int>, bool> connected_components<binary_image_t, ComponentVisitor>::tracer(pixel_index p0, int d0, int label, bool isExternal)
		{
			Idx p = p0 + m_directions[d0];
			const int connectivity = 8;
			int i = 0;
			while (m_image(p.J, p.I) != 0 && i++ < connectivity)
			{
				m_labels(p.J, p.I) = -1;
				d0 = (d0 + 1) % connectivity;
				p = p0 + m_directions[d0];
			}
			m_labels(p.J, p.I) = label;
			if (isExternal) {
				m_component_visitor.add_external_vertex(label, p.I, p.J);
			}
			else {
				m_component_visitor.add_internal_vertex(label, p.I, p.J);
			}
			return std::make_pair(std::make_pair(p, (d0 + 4) % 8), i <= connectivity);
		}

		template<typename binary_image_t, typename ComponentVisitor >
		void connected_components<binary_image_t, ComponentVisitor>::trace_contour(pixel_index start, int d0, int label, bool isExternal)
		{
			auto result = tracer(start, d0, label, isExternal);
			if (!result.second) {
				//isolated point
				return;
			}
			auto next = result.first;
			if (next.first != start)
			{
				std::pair<Idx, int> pos(next);
				while (true)
				{
					result = tracer(pos.first, (pos.second + 2) % 8, label, isExternal);
					if (!result.second) {
						//isolated point
						return;
					}

					pos = result.first;
					if (pos.first == start)
					{
						result = tracer(pos.first, (pos.second + 2) % 8, label, isExternal);
						if (!result.second) {
							//isolated point
							return;
						}

						pos = result.first;
						if (pos.first == next.first)
						{
							break;
						}
					}
				}
			}
		}

	}
}