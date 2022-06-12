#ifndef ASCII_ART_FILEMANAGER_H_2C4E44A715064547842F83A65D4E575C
#define ASCII_ART_FILEMANAGER_H_2C4E44A715064547842F83A65D4E575C

#include <string>
#include <optional>
#include <set>

#include <filesystem>

namespace fs = std::filesystem;

/**
 * FileManager class providing basic file management functions.
 */
class FileManager {
  public:
    /**
     * Get home folder based on the current user and ENV variables.
     * @return Home folder path.
     */
    static std::string & get_home();

    /**
     * Normalize path. removes redundant slashes.
     * @param path to be normalized.
     * @param remove_home if true, replaces ~ with home folder path.
     * @return normalized path.
     */
    static std::string normalize_path(const std::string_view & path, bool remove_home = false);

    /**
     * @brief Regex file name search.
     * Regex is split into groups by PATH_SEPARATOR.
     * those groups are searched in the given path. If group starts with '?', it is treated as optional.<br>
     * example: <br>
     *      `/home/user/?.+/.+\.txt`<br>
     * will match:<br>
     *      Any file in /home/user/(any directory)/(any directory) with extension .txt<br>
     *      or any file in /home/user/(any directory) with extension .txt<br>
     * like:<br>
     *     /home/user/test/test.txt<br>
     *     /home/user/test/test2/test.txt<br>
     *     /home/user/test/test2/test2.txt<br>
     * The regex matching is case sensitive.<br>
     * @param regex to be matched against file paths.
     * @return set of file paths matching the regex.
     */
    static std::set<fs::path> find_files(const std::string_view & regex);

    /**
       * Default file path separator.
       */
    static constexpr char PATH_SEPARATOR = '/'; // TODO: windows support

    static void removeRegexPart(std::string & regex, size_t & current_position);

  private:
    /**
     * Tests if given char is file path separator.
     * @param c char to be tested.
     * @return true if c is file path separator.
     */
    static bool is_separator(char c);

    /**
     * Caches the home folder path.
     */
    static std::optional<std::string> home;
    static constexpr size_t MAXIMUM_VISITED_FILES = 1000;
};


#endif //ASCII_ART_FILEMANAGER_H_2C4E44A715064547842F83A65D4E575C
