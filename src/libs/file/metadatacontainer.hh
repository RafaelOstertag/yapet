#ifndef _METADATACONTAINER_HH
#define _METADATACONTAINER_HH

namespace yapet {
template <class T>
class MetaDataContainer {
   public:
    using meta_data_type = T;

   private:
    T _data;

   public:
    MetaDataContainer(T data) : _data{data} {}
    MetaDataContainer() : _data{} {}

    MetaDataContainer(const MetaDataContainer<T>& other) : _data{other._data} {}
    MetaDataContainer(MetaDataContainer<T>&& other)
        : _data{move(other._data)} {}

    MetaDataContainer<T>& operator=(const MetaDataContainer<T>& other) {
        if (this == &other) return *this;

        _data = other._data;

        return *this;
    }

    MetaDataContainer<T>& operator=(MetaDataContainer<T>&& other) {
        if (this == &other) return *this;

        _data = move(other._data);

        return *this;
    }

    bool operator==(const MetaDataContainer<T> other) const {
        return _data == other._data;
    }
};
}  // namespace yapet

#endif