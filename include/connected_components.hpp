#pragma once

#include <algorithm>
#include <utility>
#include <vector>

namespace algodynamic {

struct pixel_index {
  int I;
  int J;
  pixel_index(int row, int column) : I(column), J(row) {}
  pixel_index operator+(const pixel_index &idx) {
    return pixel_index(idx.J + J, idx.I + I);
  }
  bool operator==(const pixel_index &idx) const {
    return idx.I == I && idx.J == J;
  }

  bool operator!=(const pixel_index &idx) const { return !(operator==(idx)); }
};

class empty_image_exception : public std::runtime_error
{
public:
	empty_image_exception() : std::runtime_error("The supplied image was empty") {}
};

template <typename Binary_image>
	// Note - this is Concept syntax. It is commented out until compiler support is available
	// (See Cpp Core Guidelines T.10 http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rt-concept)
	// requires Raster_image<Binary_image>
class connected_components {
  typedef pixel_index Idx;
  static Idx m_directions[];
  std::vector<int> m_labels;
  void trace_contour(pixel_index, int, int, bool is_external);

  std::pair<std::pair<pixel_index, int>, bool> tracer(pixel_index, int, int,
                                                      bool is_external);

public:
  explicit connected_components(const Binary_image &img);
  int foo() {};
  size_t size() const { return 0; }
};

template<typename Binary_image>
connected_components<Binary_image>::connected_components(const Binary_image & img)
{
	if (img.width() == 0 || img.height() == 0)
	{
		throw empty_image_exception();
	}
}


template <typename Binary_image>
connected_components<Binary_image>::connected_components(
    const Binary_image &img )
    : m_labels(img.width()*img.height()) {
  int last_row_index = m_image.rows() - 1;
  int last_column_index = m_image.cols() - 1;
  int height = m_image.height();
  int width = m_image.width();
  int label = 1;

  for (int j = 0; j < h; ++j) {
    auto pRow = m_image.row(j);
    pRow[0] = pRow[last_column_index] = 255;
  }

  for (int j = 1; j < last_row_index; ++j) {
    auto pSrcAbove = m_image.row(j - 1);
    auto pSrc = m_image.row(j);
    auto pSrcBelow = m_image.row(j + 1);
    auto pLbl = m_labels.row(j);
    auto pLblBelow = m_labels.row(j + 1);
    for (int i = 1; i < last_column_index; ++i) {
      // Find first black pixel
      if (pSrc[i] == 0) {
        // P is unlabeled and pixel above is white
        if (pLbl[i] == 0 && pSrcAbove[i] == 255) {
          // new external contour of new component
          m_component_visitor.init(label);
          trace_contour(Idx(j, i), 7, label, true);

          label += 1;
        }
        // pixel below P is unmarked white pixel
        else if (pLblBelow[i] == 0 && pSrcBelow[i] == 255) {
          // already labeled
          int l = 0;
          if (pLbl[i] != 0) {
            // external and internal contour
            l = pLbl[i];
          } else {
            // point immediately to the left is labeled
            l = pLbl[i - 1];
          }
          trace_contour(Idx(j, i), 3, l, false);
        } else if (pLbl[i] == 0) {
          // not a contour point
          pLbl[i] = pLbl[i - 1];
        }
      }
    }
  }
}

template <typename binary_image_t, typename ComponentVisitor>
pixel_index
    connected_components<binary_image_t, ComponentVisitor>::m_directions[] = {
        Idx(0, 1),  Idx(1, 1),   Idx(1, 0),  Idx(1, -1),
        Idx(0, -1), Idx(-1, -1), Idx(-1, 0), Idx(-1, 1)};

template <typename binary_image_t, typename ComponentVisitor>
std::pair<std::pair<pixel_index, int>, bool>
connected_components<binary_image_t, ComponentVisitor>::tracer(
    pixel_index p0, int d0, int label, bool isExternal) {
  Idx p = p0 + m_directions[d0];
  const int connectivity = 8;
  int i = 0;
  while (m_image(p.J, p.I) != 0 && i++ < connectivity) {
    m_labels(p.J, p.I) = -1;
    d0 = (d0 + 1) % connectivity;
    p = p0 + m_directions[d0];
  }
  m_labels(p.J, p.I) = label;
  if (isExternal) {
    m_component_visitor.add_external_vertex(label, p.I, p.J);
  } else {
    m_component_visitor.add_internal_vertex(label, p.I, p.J);
  }
  return std::make_pair(std::make_pair(p, (d0 + 4) % 8), i <= connectivity);
}

template <typename binary_image_t, typename ComponentVisitor>
void connected_components<binary_image_t, ComponentVisitor>::trace_contour(
    pixel_index start, int d0, int label, bool isExternal) {
  auto result = tracer(start, d0, label, isExternal);
  if (!result.second) {
    // isolated point
    return;
  }
  auto next = result.first;
  if (next.first != start) {
    std::pair<Idx, int> pos(next);
    while (true) {
      result = tracer(pos.first, (pos.second + 2) % 8, label, isExternal);
      if (!result.second) {
        // isolated point
        return;
      }

      pos = result.first;
      if (pos.first == start) {
        result = tracer(pos.first, (pos.second + 2) % 8, label, isExternal);
        if (!result.second) {
          // isolated point
          return;
        }

        pos = result.first;
        if (pos.first == next.first) {
          break;
        }
      }
    }
  }
}
*/
}