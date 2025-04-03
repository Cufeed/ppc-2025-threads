#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"
#include "core/util/include/util.hpp"
#include "tbb/gusev_n_sorting_int_simple_merging/include/ops_tbb.hpp"

namespace {
ppc::core::TaskDataPtr CreateTaskData(std::vector<int>& input, std::vector<int>& output) {
  auto task_data = std::make_shared<ppc::core::TaskData>();

  task_data->inputs.emplace_back(reinterpret_cast<uint8_t*>(input.data()));
  task_data->inputs_count.emplace_back(input.size());
  task_data->outputs.emplace_back(reinterpret_cast<uint8_t*>(output.data()));
  task_data->outputs_count.emplace_back(output.size());

  return task_data;
}

void RunT(ppc::core::TaskDataPtr& task_data) {
  gusev_n_sorting_int_simple_merging_tbb::SortingIntSimpleMergingTBB task(task_data);
  ASSERT_TRUE(task.Validation());
  ASSERT_TRUE(task.PreProcessing());
  ASSERT_TRUE(task.Run());
  ASSERT_TRUE(task.PostProcessing());
}
}  // namespace

TEST(gusev_n_sorting_int_simple_merging_tbb, test_radix_sort_basic) {
  std::vector<int> in = {170, 45, 75, 90, 802, 24, 2, 66};
  std::vector<int> out(in.size());

  auto task_data = CreateTaskData(in, out);
  RunT(task_data);

  std::vector<int> expected = in;
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, out);
}

TEST(gusev_n_sorting_int_simple_merging_tbb, test_radix_sort_empty) {
  std::vector<int> in;
  std::vector<int> out;

  auto task_data = CreateTaskData(in, out);
  RunT(task_data);

  EXPECT_TRUE(out.empty());
}

TEST(gusev_n_sorting_int_simple_merging_tbb, test_radix_sort_single_element) {
  std::vector<int> in = {42};
  std::vector<int> out(in.size());

  auto task_data = CreateTaskData(in, out);
  RunT(task_data);

  EXPECT_EQ(in, out);
}

TEST(gusev_n_sorting_int_simple_merging_tbb, test_radix_sort_negative_numbers) {
  std::vector<int> in = {3, -1, 0, -5, 2, -3};
  std::vector<int> out(in.size());

  auto task_data = CreateTaskData(in, out);
  RunT(task_data);

  std::vector<int> expected = in;
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, out);
}

TEST(gusev_n_sorting_int_simple_merging_tbb, test_radix_sort_random) {
  size_t size = 1000;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(-10000, 10000);

  std::vector<int> in(size);
  std::generate(in.begin(), in.end(), [&]() { return dist(gen); });
  std::vector<int> out(in.size());

  auto task_data = CreateTaskData(in, out);
  RunT(task_data);

  std::vector<int> expected = in;
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, out);
}

TEST(gusev_n_sorting_int_simple_merging_tbb, test_radix_sort_duplicates) {
  std::vector<int> in = {5, 3, 5, -2, 3, -2, -2, 5, 0, 0, 7, 7, -7};
  std::vector<int> out(in.size());

  auto task_data = CreateTaskData(in, out);
  RunT(task_data);

  std::vector<int> expected = in;
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, out);
}

TEST(gusev_n_sorting_int_simple_merging_tbb, test_radix_sort_reverse_order) {
  std::vector<int> in = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3};
  std::vector<int> out(in.size());

  auto task_data = CreateTaskData(in, out);
  RunT(task_data);

  std::vector<int> expected = in;
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, out);
}
