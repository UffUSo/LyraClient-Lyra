#pragma once

#include <utility>
#include <vector>
#include <ios>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Packet.hpp"

namespace mce {
    class UUID {
    private:
        uint64_t a; // Least significant bits
        uint64_t b; // Most significant bits

    public:
        UUID(uint64_t a, uint64_t b) : a(a), b(b) {}

        static UUID fromString(const std::string &uuidString) {
            // Remove hyphens from the UUID string
            std::string uuidStringWithoutHyphens;
            for (char c : uuidString) {
                if (c != '-') {
                    uuidStringWithoutHyphens.push_back(c);
                }
            }

            // Convert the string to bytes
            std::istringstream iss(uuidStringWithoutHyphens);
            std::string byteString;
            uint8_t bytes[16];
            for (int i = 0; i < 16; ++i) {
                byteString.push_back(iss.get());
                byteString.push_back(iss.get());
                std::istringstream(byteString) >> std::hex >> bytes[i];
                byteString.clear();
            }

            // Reverse byte order for little-endian systems
            uint64_t a = 0;
            uint64_t b = 0;
            for (int i = 0; i < 8; ++i) {
                a |= static_cast<uint64_t>(bytes[i]) << (8 * i);
                b |= static_cast<uint64_t>(bytes[i + 8]) << (8 * i);
            }

            return {a, b};
        }

        [[nodiscard]] std::string toString() const {
            std::stringstream ss;
            ss << std::hex << std::setw(16) << std::setfill('0') << a << '-';
            ss << std::hex << std::setw(16) << std::setfill('0') << b;
            return ss.str();
        }
    };
}

class SemVersion {
public:
    unsigned short mMajor;
    unsigned short mMinor;
    unsigned short mPatch;
    std::string mPreRelease;
    std::string mBuildMeta;
    std::string mFullVersionString;
    bool mValidVersion;
    bool mAnyVersion;

    SemVersion() : mMajor(0), mMinor(0), mPatch(0), mValidVersion(false), mAnyVersion(false) {}

    void fromString(const std::string& versionString) {
        std::istringstream ss(versionString);
        char dot;
        ss >> mMajor >> dot >> mMinor >> dot >> mPatch;

        if (!ss.fail()) {
            mValidVersion = true;
            mFullVersionString = versionString;
        } else {
            mValidVersion = false;
        }

        // If there's more after the patch version, parse pre-release and build metadata
        if (!ss.fail()) {
            std::getline(ss, mPreRelease, '.');
            std::getline(ss, mBuildMeta, '.');
        }
    }
};

enum class PackType : signed char {
    Invalid       = 0x0,
    Addon         = 0x1,
    Cached        = 0x2,
    CopyProtected = 0x3,
    Behavior      = 0x4,
    PersonaPiece  = 0x5,
    Resources     = 0x6,
    Skins         = 0x7,
    WorldTemplate = 0x8,
    Count         = 0x9,
};

struct PackIdVersion {
public:
    mce::UUID mId;
    SemVersion mVersion;
    PackType mPackType;

    PackIdVersion(const std::string& uuidString, const std::string& versionString, PackType packType)
            : mId(mce::UUID::fromString(uuidString)), mVersion(), mPackType(packType) {
        mVersion.fromString(versionString);
    }
};

class ContentIdentity {
public:
    mce::UUID uuid;
    bool valid;

    ContentIdentity(const std::string& uuidString, bool isValid)
            : uuid(mce::UUID::fromString(uuidString)), valid(isValid) {}
};

struct PackInfoData {
public:
    PackInfoData(const std::string& uuidString, const std::string& versionString, std::string  contentKey, uint64_t mPackSize, PackType packType)
            : mPackIdVersion(uuidString, versionString, packType), mPackSize(mPackSize), mContentKey(std::move(contentKey)),
              mContentIdentity(uuidString, true), mHasScripts(false), mIsRayTracingCapable(true), mHasExceptions(false) {}

    PackIdVersion   mPackIdVersion;
    uint64_t        mPackSize;
    std::string     mContentKey;
    std::string     mSubpackName;
    ContentIdentity mContentIdentity;
    bool            mHasScripts;
    bool            mIsRayTracingCapable;
    bool            mHasExceptions;
};

struct PacksInfoData {
public:
    bool                      mResourcePackRequired;
    bool                      mHasScripts;
    bool                      mHasExceptions;
    bool                      mForceServerPacksEnabled;
    std::vector<PackInfoData> mBehaviorPacks;
    std::vector<PackInfoData> mResourcePacks;

    PacksInfoData() : mResourcePackRequired(false), mHasScripts(false), mHasExceptions(false),
                      mForceServerPacksEnabled(false) {}
};

class ResourcePacksInfoPacket : public Packet {
public:
    ResourcePacksInfoPacket() {}

    PacksInfoData mData;
    std::vector<std::pair<std::string, std::string>> mCDNUrls;

    void printCDNUrls() const {
        std::cout << "CDN URLs:\n";
        for (const auto& urlPair : mCDNUrls) {
            std::cout << "Name: " << urlPair.first << ", URL: " << urlPair.second << std::endl;
        }
    }

    void addPack(const PackInfoData& packInfo) {
        if (packInfo.mPackIdVersion.mPackType == PackType::Resources) {
            mData.mResourcePacks.push_back(packInfo);
        } else if (packInfo.mPackIdVersion.mPackType == PackType::Behavior) {
            mData.mBehaviorPacks.push_back(packInfo);
        } else {
            // Handle other pack types if necessary
            //std::cerr << "Unsupported pack type!" << std::endl;
            mData.mResourcePacks.push_back(packInfo);
        }
    }
};