#ifndef ASCII_ART_FILEMANAGER_H_2C4E44A715064547842F83A65D4E575C
#define ASCII_ART_FILEMANAGER_H_2C4E44A715064547842F83A65D4E575C

#include <string>
#include <optional>
#include <set>

#include <filesystem>
namespace fs = std::filesystem;

class FileManager {
  public:
    static std::string & get_home();

    static std::string normalize_path(const std::string_view & path, bool remove_home = false);
    static std::set<fs::path> find_files(const std::string_view & regex);

  private:
    static bool is_separator(char c);
    static std::optional<std::string> home;

    static constexpr char PATH_SEPARATOR = '/'; // TODO: windows support
};


#endif //ASCII_ART_FILEMANAGER_H_2C4E44A715064547842F83A65D4E575C
