#ifndef _RNG_HH
#define _RNG_HH

#include <cstdint>
#include <memory>
#include <string>

namespace yapet {
namespace pwgen {
class RngInterface {
   public:
    virtual ~RngInterface() {}
    virtual std::uint8_t getNextByte() = 0;
};

class RngFile : public RngInterface {
   private:
    std::string _filename;
    int fd;
    void closeFd();

   public:
    RngFile(const std::string& filename);
    ~RngFile();
    RngFile(const RngFile& rng);
    RngFile(RngFile&& rng);
    RngFile& operator=(const RngFile& rng);
    RngFile& operator=(RngFile&& rng);

    const std::string& filename() const { return _filename; }

    virtual std::uint8_t getNextByte();
};

class RngRand : public RngInterface {
   public:
    virtual std::uint8_t getNextByte();
};

enum RNGENGINE {
    DEVRANDOM = (1 << 0),
    DEVURANDOM = (1 << 1),
    RAND = (1 << 2)
};

std::unique_ptr<RngInterface> getRng(RNGENGINE rngEngine);

}  // namespace pwgen
}  // namespace yapet

#endif