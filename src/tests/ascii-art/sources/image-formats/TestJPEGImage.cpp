#include "TestJPEGImage.h"
#include <iostream>
#include "../../../../ascii-art/sources/image-formats/JPEGImage.h"
#include "../../../../FileManager.h"


int TestJPEGImage::runTest() {
    std::set<fs::path> paths = FileManager::find_files("./examples/(jpeg|jpg)/corrupt/.*\\.(jpeg|jpg)");
    testAssert(!paths.empty());
    for (const auto & item : paths) {
        try {
            JPEGImage img(item);
            std::cout << "Testing corrupt: " << item << std::endl;
            img.getFrame(0);
            testAssert(false); // should throw exception since file is corrupt
        } catch (std::exception & ex) {
        }
    }
    paths = FileManager::find_files("./examples/(jpeg|jpg)/.*\\.(jpeg|jpg)");
    testAssert(!paths.empty());
    for (const auto & item : paths) {
        try {
            JPEGImage img(item);
            std::cout << "Testing ok file: " << item << std::endl;
            Frame & frame = img.getFrame(0);
            testAssert(frame.getSize().m_x && frame.getSize().m_y);
        } catch (std::exception & ex) {
            testAssert(false); // should throw exception since file is corrupt
        }
    }
    return 0;
}
