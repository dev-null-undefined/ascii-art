#include "TestFileManager.h"
#include "../FileManager.h"
#include <string>

int TestFileManager::runTest() {
    using namespace std::string_literals;
    testAssert(FileManager::is_separator('/'));
    testAssert(!FileManager::is_separator(' '));
    testAssert(!FileManager::is_separator('='));
    std::string regex_path = "///test/t/tt//";
    std::string normalized = FileManager::normalize_path(regex_path, false);
    assertEqualString(normalized, "/test/t/tt/");

    regex_path = "~////test//////////t";
    normalized = FileManager::normalize_path(regex_path, true);
    std::string home = FileManager::get_home();
    testAssert(home.size() > 0);
    assertEqualString(normalized, home + "/test/t");

    std::set<fs::path> paths;

    paths = FileManager::find_files("");
    testAssert(paths.empty());

    paths = FileManager::find_files("~");
    testAssert(paths.empty());

    paths = FileManager::find_files("/");
    testAssert(paths.empty());

    regex_path = "/aaa/bbb/ccc/ddd";
    //                 ^
    size_t position = 5;
    FileManager::removeRegexPart(regex_path, position);
    assertEqualString(regex_path, "/aaa/ccc/ddd");
    assertEqual(position, (size_t) 4);

    regex_path = "/aaa/bbb/ccc/ddd";
    //                ^
    position = 4;
    FileManager::removeRegexPart(regex_path, position);
    assertEqualString(regex_path, "/aaa/bbb/ccc/ddd");
    assertEqual(position, (size_t) 4);


    regex_path = "/aaa/bbb/ccc/ddd";
    //                          ^
    position = 14;
    FileManager::removeRegexPart(regex_path, position);
    assertEqualString(regex_path, "/aaa/bbb/ccc");
    assertEqual(position, (size_t) 12);
    return 0;
}
