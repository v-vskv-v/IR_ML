#ifndef SERIALIZER_H_INCLUDED
#define SERIALIZER_H_INCLUDED

using namespace std;

enum class Error {
    NoError,
    CorruptedArchive
};

class Serializer {
    ostream& out_;
    static constexpr char Separator = ' ';

public:
    explicit Serializer(ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(forward<ArgsT>(args)...);
    }

private:
    void process(bool element)
    {
        if (element)
            out_ << "true";
        else
            out_ << "false";
    }

    void process(uint64_t element)
    {
        out_ << element;
    }

    template <class T, class... ArgsT>
    Error process(T&& head, ArgsT&&... tail)
    {
        process(forward<T>(head));
        out_ << Separator;
        process(forward<ArgsT>(tail)...);
        return Error::NoError;
    }
};

class Deserializer {
    istream& in_;

public:
    explicit Deserializer(istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(forward<ArgsT>(args)...);
    }

private:
    Error process(bool& element)
    {
        string text;
        in_ >> text;

        if (text == "true")
            element = true;
        else if (text == "false")
            element = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error process(uint64_t& element)
    {
        string text;
        in_ >> text;

        if (text.empty())
            return Error::CorruptedArchive;

        element = 0;

        for (unsigned i = 0; i < text.length(); i++) {
            if (text[i] > '9' || text[i] < '0')
                return Error::CorruptedArchive;
            element *= 10;
            element += text[i] - '0';
        }
        return Error::NoError;
    }

    template <class T, class... ArgsT>
    Error process(T&& head, ArgsT&&... tail)
    {
        if (process(forward<T>(head)) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(forward<ArgsT>(tail)...);
    }
};
#endif // SERIALIZER_H_INCLUDED
