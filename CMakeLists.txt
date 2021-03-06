cmake_minimum_required(VERSION 3.17)
project(MyGit)

find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(MyGit
        src/Main.cpp
        src/Files/BaseFile.hpp src/Files/SourceFile.cpp src/Files/SourceFile.hpp src/Files/Directory.cpp src/Files/Directory.hpp src/Files/DirectoryEntry.cpp src/Files/DirectoryEntry.hpp
        src/Enums/FileType.hpp
        src/Hashing/HashingFunctions.hpp
        src/List/List.hpp src/List/List.inl src/List/ListIterator.hpp src/List/ListIterator.inl src/List/ListNode.hpp src/Files/SourceFile.cpp src/Files/SourceFile.hpp src/Files/Directory.cpp src/Files/Directory.hpp src/List/ListConstIterator.hpp src/List/ListConstIterator.inl
        src/HashMap/HashMap.hpp src/HashMap/HashMapIterator.hpp src/HashMap/HashMapIterator.inl src/HashMap/HashMap.inl src/Repository/Repository.cpp src/Repository/Repository.hpp src/Commit/Commit.cpp src/Commit/Commit.hpp src/HashMap/HashMapConstIterator.hpp src/HashMap/HashMapConstIterator.inl src/Repository/RepositoryMetaInfo.hpp src/CommandLineParser/Parser.cpp src/CommandLineParser/Parser.hpp src/CommandLineParser/Command.hpp src/CommandLineParser/InitCommand.cpp src/CommandLineParser/StatusCommand.cpp src/CommandLineParser/CommitCommand.cpp src/CommandLineParser/CheckoutCommand.cpp src/CommandLineParser/RevertCommand.cpp src/CommandLineParser/LogCommand.cpp)

target_link_libraries(MyGit stdc++fs)
target_link_libraries(MyGit OpenSSL::SSL)