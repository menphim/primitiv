#ifndef PRIMITIV_CUDA_DEVICE_H_
#define PRIMITIV_CUDA_DEVICE_H_

#include <cuda_runtime_api.h>
#include <cublas_v2.h>
#include <curand.h>
#include <map>
#include <primitiv/device.h>

namespace primitiv {

/**
 * Device class for CUDA.
 */
class CUDADevice : public Device {
  CUDADevice() = delete;
  CUDADevice(const CUDADevice &) = delete;
  CUDADevice(CUDADevice &&) = delete;
  CUDADevice &operator=(const CUDADevice &) = delete;
  CUDADevice &operator=(CUDADevice &&) = delete;

public:
  /**
   * Creates a new CUDA device.
   * @param device_id ID of the physical GPU.
   * @remarks The random number generator is initialized using
   *          `std::random_device`.
   */
  explicit CUDADevice(unsigned device_id);

  /**
   * Creates a new CUDA device.
   * @param device_id ID of the physical GPU.
   * @param rng_seed The seed value of the random number generator.
   */
  CUDADevice(unsigned device_id, unsigned rng_seed);

  ~CUDADevice() override;

private:
  Tensor new_tensor_impl(const Shape &shape) override;
  void delete_tensor_impl(Tensor &x) override;
  
  std::vector<float> tensor_to_vector_impl(const Tensor &x) override;
  
  void reset_tensor_impl(Tensor &x, float k) override;
  void reset_tensor_impl(Tensor &x, const std::vector<float> &values) override;

  Tensor random_bernoulli_impl(const Shape &shape, float p) override;
  Tensor random_uniform_impl(
      const Shape &shape, float lower, float upper) override;
  Tensor random_normal_impl(const Shape &shape, float mean, float sd) override;

  Tensor slice_impl(
      const Tensor &x, unsigned dim, unsigned lower, unsigned upper) override;
  Tensor concat_impl(
      const std::vector<const Tensor *> &xs,
      unsigned dim, const Shape &new_shape) override;

  Tensor duplicate_impl(const Tensor &x) override;
  Tensor negate_impl(const Tensor &x) override;

  Tensor add_impl(const Tensor &x, float k) override;
  Tensor add_impl(const Tensor &a, const Tensor &b) override;
  Tensor subtract_impl(const Tensor &x, float k) override;
  Tensor subtract_impl(float k, const Tensor &x) override;
  Tensor subtract_impl(const Tensor &a, const Tensor &b) override;
  Tensor multiply_impl(const Tensor &x, float k) override;
  Tensor multiply_impl(const Tensor &a, const Tensor &b) override;
  Tensor divide_impl(const Tensor &x, float k) override;
  Tensor divide_impl(float k, const Tensor &x) override;
  Tensor divide_impl(const Tensor &a, const Tensor &b) override;

  Tensor transpose_impl(const Tensor &x) override;
  Tensor dot_impl(const Tensor &a, const Tensor &b) override;

  Tensor exp_impl(const Tensor &x) override;
  Tensor tanh_impl(const Tensor &x) override;
  Tensor sigmoid_impl(const Tensor &x) override;
  Tensor step_impl(const Tensor &x) override;
  Tensor relu_impl(const Tensor &x) override;

  Tensor batch_sum_impl(const Tensor &x) override;

  void add_gradient_impl(Tensor &a, const Tensor &b) override;

private:
  unsigned dev_id_;
  unsigned rng_seed_;
  unsigned dim1_x_;
  unsigned dim2_x_;
  unsigned dim2_y_;
  std::map<void *, unsigned> blocks_;
  ::cudaDeviceProp prop_;
  ::cublasHandle_t cublas_;
  ::curandGenerator_t curand_;

  /**
   * Internal method to initialize the object.
   */
  void initialize();
};

}  // namespace primitiv

#endif  // PRIMITIV_CUDA_DEVICE_H_