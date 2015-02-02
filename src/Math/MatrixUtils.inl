#include <sstream>
#include <Core/EigenTypedef.h>
#include <Core/Exception.h>

template<typename T>
T MatrixUtils::vstack(const std::vector<T>& matrices) {
    if (matrices.empty()) {
        throw RuntimeError("Cannot vstack empty matrix array");
    }
    size_t num_rows=0;
    size_t num_cols=matrices[0].cols();
    for (const auto& mat : matrices) {
        num_rows += mat.rows();
        if (mat.cols() != num_cols) {
            std::stringstream err_msg;
            err_msg << "Cannot vstack because "
                << "matrix have different number of columns"
                << std::endl
                << "Expect " << num_cols << " but get "
                << mat.cols() << "columns";
            throw RuntimeError(err_msg.str());
        }
    }

    T result(num_rows, num_cols);
    size_t row_count = 0;
    for (const auto& mat : matrices) {
        result.block(row_count, 0, mat.rows(), mat.cols()) = mat;
        row_count += mat.rows();
    }
    assert(row_count == num_rows);
    return result;
}

template<typename T>
Eigen::Matrix<typename T::Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>
MatrixUtils::rowstack(const std::vector<T>& rows) {
    typedef Eigen::Matrix<typename T::Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> OutType;
    if (rows.empty()) {
        throw RuntimeError("Zero rows to stack");
    }

    const size_t num_rows = rows.size();
    const size_t num_cols = rows.front().size();

    OutType result(num_rows, num_cols);

    size_t row_count = 0;
    for (const auto& row : rows) {
        if (row.size() != num_cols) {
            std::stringstream err_msg;
            err_msg << "Expect vectors are of size "
                << num_cols << " but get " << row.size()
                << " at row " << row_count;
            throw RuntimeError(err_msg.str());
        }

        std::copy(row.data(), row.data() + num_cols,
                result.data() + row_count * num_cols);
        row_count++;
    }

    return result;
}

template<typename T>
Eigen::Matrix<typename T::Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>
MatrixUtils::colstack(const std::vector<T>& cols) {
    typedef Eigen::Matrix<typename T::Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> OutType;
    if (cols.empty()) {
        throw RuntimeError("Zero cols to stack");
    }

    const size_t num_rows = cols.front().size();
    const size_t num_cols = cols.size();

    OutType result(num_rows, num_cols);

    size_t col_count = 0;
    for (const auto& col : cols) {
        if (col.size() != num_rows) {
            std::stringstream err_msg;
            err_msg << "Expect vectors are of size "
                << num_rows << " but get " << col.size()
                << " at row " << col_count;
            throw RuntimeError(err_msg.str());
        }

        std::copy(col.data(), col.data() + num_rows,
                result.data() + col_count * num_rows);
        col_count++;
    }

    return result;
}

template<typename T>
Eigen::Matrix<T, Eigen::Dynamic, 1> MatrixUtils::std2eigen(
        const std::vector<T>& data) {
    const size_t num_data = data.size();
    Eigen::Matrix<T, Eigen::Dynamic, 1> result(num_data);
    std::copy(data.begin(), data.end(), result.data());
    return result;
}
