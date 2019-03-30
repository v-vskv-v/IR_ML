#ifndef MATRIX_H
#define MATRIX_H


class Matrix {
private:
    unsigned rows;
    unsigned cols;
    int32_t **data;
public:
    Matrix (unsigned inrows, unsigned incols) {
        rows = inrows;
        cols = incols;
        //if (inrows < 0 || incols < 0)
        //    throw std::out_of_range("");
        data = new int32_t*[rows];
        for (unsigned i = 0; i < rows; i++)
            data[i] = new int32_t[cols];
    }

    ~Matrix () {}

    unsigned getRows() const {
        return this->rows;
    }

    unsigned getColumns() const {
        return this->cols;
    }

    class Row {
    public:
        int32_t *data;
        unsigned cols;

        Row (int32_t *row, int32_t c)
            :data (row), cols (c) {}


        int32_t& operator [] (size_t j) {
            if (j >= cols || j < 0)
                throw std::out_of_range("");
            return this->data[j];
        }
    };

    Row operator [] (size_t i) const {
        if (i >= this->rows || i < 0)
            throw std::out_of_range("");
        Row row (this->data[i], this->cols);
        return row;
    }

    bool operator == (const Matrix& other) const {
        if (this == &other)
            return 1;
        if (this->rows != other.rows || this->cols != other.cols)
            return 0;
        for (unsigned i = 0; i < this->rows; i++) {
            for (unsigned j = 0; j < this->cols; j++) {
                if (other.data[i][j] != this->data[i][j])
                    return 0;
            }
        }
        return 1;
    }

    bool operator != (const Matrix& other) const {
        return !(*this == other);
    }

    const Matrix& operator *= (const size_t value) {
        for (unsigned i = 0; i < this->rows; i++) {
            for (unsigned j = 0; j < this->cols; j++) {
                this->data[i][j] *= value;
            }
        }
        return *this;
    }
};

#endif // MATRIX_H_INCLUDED
