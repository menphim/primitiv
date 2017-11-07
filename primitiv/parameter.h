#ifndef PRIMITIV_PARAMETER_H_
#define PRIMITIV_PARAMETER_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <primitiv/device.h>
#include <primitiv/error.h>
#include <primitiv/mixins.h>
#include <primitiv/shape.h>
#include <primitiv/tensor.h>

namespace primitiv {

class Initializer;

/**
 * Class to manage a trainable tensor parameter.
 */
class Parameter : mixins::Noncopyable<Parameter> {
public:
  Parameter(Parameter &&src)
    : shape_(std::move(src.shape_))
    , device_(src.device_)
    , value_(std::move(src.value_))
    , grad_(std::move(src.grad_))
    , stats_(std::move(src.stats_)) {
      src.device_ = nullptr;
    }

  Parameter &operator=(Parameter &&src) {
    if (&src != this) {
      shape_ = std::move(src.shape_);
      device_ = src.device_;
      value_ = std::move(src.value_);
      grad_ = std::move(src.grad_);
      stats_ = std::move(src.stats_);
      src.device_ = nullptr;
    }
    return *this;
  }

  /**
   * Creates an invalid parameter object.
   */
  Parameter() : shape_(), device_(nullptr), value_(), grad_() {}

  /**
   * Creates a new Parameter object.
   * @param shape The shape of the parameter. The batch size should be 1.
   * @param value List of initial values. Order of elements should be of
   *              `Tensor::set_values()`.
   * @param device The device object to manage internal memory.
   */
  Parameter(
      const Shape &shape,
      const std::vector<float> &value,
      Device &device = Device::get_default());

  /**
   * Creates a new Parameter object.
   * @param shape The shape of the parameter. The batch size should be 1.
   * @param init An Initializer object.
   * @param device The device object to manage internal memory.
   */
  Parameter(
      const Shape &shape,
      const Initializer &init,
      Device &device = Device::get_default());

  /**
   * Returns whether the parameter is valid or not.
   * @return true or false w.r.t. the parameter is valid or not.
   */
  bool valid() const { return !!device_; }

  /**
   * Set all values.
   * @param value List of new parameter values. Order of the values should be
   *              of `Tensor::set_values()`.
   */
  void reset_value(const std::vector<float> &value);

  /**
   * Set all values using a specific initialization criteria.
   * @param init An Initializer object.
   */
  void reset_value(const Initializer &init);

  /**
   * Set all gradients to 0.
   */
  void reset_gradient();

  /**
   * Adds a new optional statistics tensor.
   * @param name Name of the statistics.
   * @param shape Shape of the tensor.
   * @remarks All elements in the new statistics tensor is initialized by 0.
   */
  void add_stats(const std::string &name, const Shape &shape);

  /**
   * Checks whether the statistics with name `name` exists or not.
   * @param name Name of the statistics.
   * @return true if the entry exists, false otherwise.
   */
  bool has_stats(const std::string &name) const {
    if (!valid()) THROW_ERROR("Invalid parameter.");
    return stats_.find(name) != stats_.end();
  }

  /**
   * Returns the shape of the parameter.
   * @return Shape object.
   */
  const Shape &shape() const {
    if (!valid()) THROW_ERROR("Invalid parameter.");
    return shape_;
  }

  /**
   * Returns the Device object to manage the internal memory.
   * @return Pointer of the Device object.
   */
  Device &device() const {
    if (!valid()) THROW_ERROR("Invalid parameter.");
    return *device_;
  }

  /**
   * Returns the values of the parameter.
   * @return A tensor representing the parameter tensor.
   */
  const Tensor &value() const {
    if (!valid()) THROW_ERROR("Invalid parameter.");
    return value_;
  }

  /**
   * Returns the values of the parameter.
   * @return A tensor representing the parameter tensor.
   */
  Tensor &value() {
    if (!valid()) THROW_ERROR("Invalid parameter.");
    return value_;
  }

  /**
   * Returns the current gradient of the parameter.
   * @return A tensor representing the gradient of the value.
   */
  const Tensor &gradient() const {
    if (!valid()) THROW_ERROR("Invalid parameter.");
    return grad_;
  }

  /**
   * Returns the current gradient of the parameter.
   * @return A tensor representing the gradient of the value.
   */
  Tensor &gradient() {
    if (!valid()) THROW_ERROR("Invalid parameter.");
    return grad_; }

  /**
   * Returns the current opotional statistics tensor specified by given name.
   * @param name Name of the statistics.
   * @return A tensor.
   */
  const Tensor &stats(const std::string &name) const {
    if (!valid()) THROW_ERROR("Invalid parameter.");
    return stats_.at(name);
  }

  /**
   * Returns the current opotional statistics tensor specified by given name.
   * @param name Name of the statistics.
   * @return A tensor.
   */
  Tensor &stats(const std::string &name) {
    if (!valid()) THROW_ERROR("Invalid parameter.");
    return stats_.at(name);
  }

  /**
   * Saves current parameters into specified file with YAML format.
   * @param path File path to write parameters.
   * @param with_stats Whether or not to save all additional statistics as well
   *                   as parameter values if the parameter object has them.
   */
  void save(const std::string &path, bool with_stats = true) const;

  /**
   * Loads parameters and returns a new Parameter object.
   * @param path File path to load parameters.
   * @param with_stats Whether or not to load all additional statistics as well
   *                   as parameter values if the file contains them.
   * @param device Device object to manage internal memories.
   * @return A new Parameter object.
   */
  static Parameter load(
      const std::string &path,
      bool with_stats = true,
      Device &device = Device::get_default());

private:
  /**
   * Makes a Parameter object directly from its values.
   * @param value Value of the parameter.
   * @param stats Map of optional statistics.
   */
  void initialize_by_data(
      Tensor &&value,
      std::unordered_map<std::string, Tensor> &&stats);

  /**
   * Checks the shape of the parameter.
   */
  void check_shape();

  Shape shape_;
  Device *device_;
  Tensor value_;
  Tensor grad_;
  std::unordered_map<std::string, Tensor> stats_;
};

}  // namespace primitiv

#endif  // PRIMITIV_PARAMETER_H_
