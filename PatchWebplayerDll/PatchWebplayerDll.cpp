/*
 * AUTHOR SternW Zhang(sternwzhang@outlook.com)
*/

#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

#define WARNING_MSG(x) cout << "[WARNING] : " << __func__ << "@" << __LINE__ << " : " << x << endl;
#define FAILED_MSG(x)  cout << "[FAILED]  : " << __func__ << "@" << __LINE__ << " : " << x << endl;
#define DEBUG_MSG(x)   cout << "[DEBUG]   : " << __func__ << "@" << __LINE__ << " : " << x << endl;

void print_usage(char *argv[]) {
    cout << " PatchWebplayerDll.exe : patch Unity3D Webplayer with custom update domain" << endl << endl;
    cout << "  USAGE:" << endl;
    cout << "\t - " << "PatchWebplayerDll.exe" << " [DLL FILE PATH] --replaced-with=[domain]" << endl;
    cout << endl;
    cout << "  EXAMPLES:" << endl;
    cout << "\t PatchWebplayerDll.exe --replaced-with=www.example.com \t // default patch the dll currently in use" << endl;
    cout << "\t PatchWebplayerDll.exe .\\npUnity3D32.dll --replaced-with=www.example.com" << endl;
}

int replace_str(char* Buffer, const char* Target, const char* replaced_with, size_t bytes_of_buffer) {
    size_t target_len = strlen(Target);
    if (target_len < strlen(replaced_with)) {
        WARNING_MSG("Replace [" << Target << "] with a longer string [" << replaced_with << "] may lead to crash");
    }
    int replaced_count = 0;
    for (size_t index = 0; index + target_len < bytes_of_buffer; ++index) {
        if (strncmp(Buffer + index, Target, target_len) == 0) {
            string str(Buffer + index);
            string ori_str(str);
            str.replace(0, target_len, replaced_with);
            DEBUG_MSG("[@ " << index << " ] Replace " << ori_str << " with " << str);
            strncpy(Buffer + index, str.c_str(), str.length());
            Buffer[index + str.length()] = 0;
            index += target_len - 1;
            replaced_count++;
            continue;
        }
    }
    return replaced_count;
}

int main(int argc, char* argv[]) {
    string dll_file_path = "";
    string replaced_with = "";
    if (argc == 2 && strnicmp(argv[1],"--replaced-with=",strlen("--replaced-with="))) {    // default dll path
        replaced_with = string(argv[1]).substr(strlen("--replaced-with=")-1);
        char* Buffer = nullptr;
        size_t bytes;
        _dupenv_s(&Buffer, &bytes, "HOMEDRIVE");
        if (Buffer == nullptr) {
            FAILED_MSG("Can't fetch environment variable 'HOMEDRIVE'");
            exit(0);
        }
        dll_file_path.append(Buffer);
        free(Buffer);
        _dupenv_s(&Buffer, &bytes, "HOMEPATH");
        if (Buffer == nullptr) {
            FAILED_MSG("Can't fetch environment variable 'HOMEDRIVER'");
            exit(0);
        }
        dll_file_path.append(Buffer);
        free(Buffer);
        dll_file_path.append("\\AppData\\LocalLow\\Unity\\WebPlayer\\loader\\npUnity3D32.dll");
    }
    else if (argc == 3 && strnicmp(argv[2], "--replaced-with=", strlen("--replaced-with="))) {   // specified dll path;
        dll_file_path.append(argv[1]);
        replaced_with = string(argv[2]).substr(strlen("--replaced-with=")-1);
    }
    else {
        print_usage(argv);
        exit(0);
    }
    DEBUG_MSG("PATCH FILE --> " << dll_file_path);
    DEBUG_MSG("PATCH WITH --> " << replaced_with << endl << endl);
    DEBUG_MSG("Read file  --> " << dll_file_path);

    fstream dll_file;
    dll_file.open(dll_file_path, ios::out | ios::in | ios::binary);
    if (!dll_file) {
        FAILED_MSG("Can't open file " << dll_file_path << " because [" << strerror(errno) << "]");
        exit(1);
    }

    dll_file.seekg(0, ios::end);
    size_t file_size = dll_file.tellg();
    dll_file.seekg(0, ios::beg);

    auto_ptr<char> dll_content(new char[file_size]);

    dll_file.read(dll_content.get(), file_size);


    dll_file.close();
    MoveFileA(dll_file_path.c_str(), (dll_file_path + string("_BACKUP")).c_str());

    dll_file.open(dll_file_path, fstream::out | fstream::binary | fstream::trunc);
    if (!dll_file) {
        FAILED_MSG("Can't open file " << dll_file_path << " because [" << strerror(errno) << "]");
        exit(1);
    }
    DEBUG_MSG("Patching file " << dll_file_path);
    int replaced_count = 0;
    replaced_count += replace_str(dll_content.get(), "autoupdate-revision.unity3d.com",replaced_with.c_str(), file_size);     // REPLACE HERE!
    replaced_count += replace_str(dll_content.get(), "webplayer.unity3d.com", replaced_with.c_str(), file_size);              // REPLACE HERE!
    replaced_count += replace_str(dll_content.get(), "ssl-", "", file_size);
    if (replaced_count) {
        string backup_file_path = dll_file_path + ("_BACKUP");
        DEBUG_MSG("Create bakcup file at " << backup_file_path);
        MoveFileA(dll_file_path.c_str(), backup_file_path.c_str());
    }
    else {
        cout << endl;
        WARNING_MSG("No file has been patched! maybe the file has been modified? try again after reinstall Unity3D Webplayer." << endl);
    }

    dll_file.write(dll_content.get(), file_size);
    dll_file.flush();
    dll_file.close();
    DEBUG_MSG("Patch done.");
}