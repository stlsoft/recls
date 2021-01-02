/* /////////////////////////////////////////////////////////////////////////
 * File:        test.scratch.cpp_api.cpp
 *
 * Purpose:     Implementation file for the test.scratch.cpp_api project.
 *
 * Created:     4th January 2010
 * Updated:     1st January 2021
 *
 * Status:      Wizard-generated
 *
 * License:     (Licensed under the Synesis Software Open License)
 *
 *              Copyright (c) 2010-2021, Synesis Software / Synesis Information Systems
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au/software
 *
 * ////////////////////////////////////////////////////////////////////// */


/* recls header files */
#include <recls/recls.hpp>

/* STLSoft header files */
#include <platformstl/platformstl.hpp>

/* Standard C++ header files */
#include <exception>
#include <iostream>
#include <iomanip>
#if 0
#include <algorithm>
#include <list>
#include <vector>
#endif /* 0 */

/* Standard C header files */
#include <stdio.h>
#include <stdlib.h>

#if defined(_MSC_VER) && \
    defined(_DEBUG)
# include <crtdbg.h>
#endif /* _MSC_VER) && _DEBUG */

/* /////////////////////////////////////////////////////////////////////////
 * forward declarations
 */

static void display_entry(recls::entry const& e);

/* ////////////////////////////////////////////////////////////////////// */

static int main_(int /* argc */, char** argv)
{
    using namespace recls;

    { // roots

        recls::cpp::root_sequence   roots;

        std::cout << "\n" << "Roots:" << std::endl;
        { for (root_sequence::const_iterator i = roots.begin(), e = roots.end(); i != e; ++i)
        {
            std::cout << "\n" << "    " << *i << std::endl;
        }}
    }

    { // stat()

        std::cout << "\n" << "stat:" << std::endl;

        {
            std::cout << "\n" << "  stat(\".\"):" << std::endl;

            recls::cpp::entry e = recls::cpp::stat(".");

            display_entry(e);
        }

        {
            std::cout << "\n" << "  stat(argv[0]):" << std::endl;

            recls::cpp::entry e = recls::cpp::stat(argv[0], recls::DIRECTORY_PARTS);

            display_entry(e);
        }

        {
            std::cout << "\n" << "  stat(\"~\"):" << std::endl;

            recls::cpp::entry e = recls::cpp::stat("~");

            display_entry(e);
        }

        {
            std::cout << "\n" << "  stat(\"/\"):" << std::endl;

            recls::cpp::entry e = recls::cpp::stat("/");

            display_entry(e);
        }
    }

    { // search_sequence

        std::cout << "\n" << "files under \"..\":" << std::endl;

        recls::cpp::search_sequence files("..", Recls_GetWildcardsAll(), recls::FILES | recls::RECURSIVE | recls::DIRECTORY_PARTS);

        { for (search_sequence::const_iterator i = files.begin(), e = files.end(); i != e; ++i)
        {
            display_entry(*i);
        }}
    }

    { // search_sequence

        std::string first_path;

        std::cout << "\n" << "custom searches, to illustrate flexibility in API:" << std::endl;

        std::cout << "  " << "search (from '~' for all files and break after first file):" << std::endl;

        recls::cpp::search_sequence files0("~/", Recls_GetWildcardsAll(), recls::FILES | recls::RECURSIVE | recls::DIRECTORY_PARTS);

        { for (search_sequence::const_iterator i = files0.begin(), e = files0.end(); i != e; )
        {
            display_entry(*i);

            recls::entry first(*i);

            first_path.assign(first.get_path().c_str());

            break;
        }}

        {
            std::cout << "  " << "search using first path as patterns (and null as search-root):" << std::endl;

            recls::cpp::search_sequence files(NULL, first_path, recls::FILES | recls::RECURSIVE | recls::DIRECTORY_PARTS);

            { for (search_sequence::const_iterator i = files.begin(), e = files.end(); i != e; ++i)
            {
                display_entry(*i);
            }}
        }

        {
            std::cout << "  " << "search using first path as patterns (and '~' as search-root):" << std::endl;

            recls::cpp::search_sequence files("~", first_path, recls::FILES | recls::RECURSIVE | recls::DIRECTORY_PARTS);

            { for (search_sequence::const_iterator i = files.begin(), e = files.end(); i != e; ++i)
            {
                display_entry(*i);
            }}
        }
    }

#ifdef RECLS_API_FTP

    { // ftp_search_sequence

        recls::cpp::ftp_search_sequence files("ftp.digitalmars.com", "", "", "/", Recls_GetWildcardsAll(), recls::FILES | recls::RECURSIVE | recls::DIRECTORY_PARTS);

        { for (ftp_search_sequence::const_iterator i = files.begin(), e = files.end(); i != e; ++i)
        {
            display_entry(*i);
        }}
    }
#endif /* RECLS_API_FTP */

    return EXIT_SUCCESS;
}

static void display_entry(recls::entry const& e)
{
    using namespace recls;

    size_t width = 25;

#ifdef RECLS_CPP_METHOD_PROPERTY_SUPPORT
    std::cout << std::setw(width) << "SearchRelativePath:" << "    " << e.SearchRelativePath << std::endl;
    std::cout << std::setw(width) << "Path:" << "    " << e.Path << std::endl;
    std::cout << std::setw(width) << "DirectoryPath:" << "    " << e.DirectoryPath << std::endl;
    std::cout << std::setw(width) << "Drive:" << "    " << e.Drive << std::endl;
    std::cout << std::setw(width) << "Directory:" << "    " << std::setw(e.get_drive().length()) << "" << e.Directory << std::endl;
    std::cout << std::setw(width) << "File:" << "    " << std::setw(e.get_directory_path().length()) << "" << e.File << std::endl;
    std::cout << std::setw(width) << "FileName:" << "    " << std::setw(e.get_directory_path().length()) << "" << e.FileName << std::endl;
    std::cout << std::setw(width) << "FileExtension:" << "    " << std::setw(e.get_directory_path().length() + e.get_file_name().length()) << "" << e.FileExtension << std::endl;
#else /* ? RECLS_CPP_METHOD_PROPERTY_SUPPORT */
    std::cout << std::setw(width) << "search-relative path:" << "    " << e.get_search_relative_path() << std::endl;
    std::cout << std::setw(width) << "path:" << "    " << e.get_path() << std::endl;
    std::cout << std::setw(width) << "directory_path:" << "    " << e.get_directory_path() << std::endl;
    std::cout << std::setw(width) << "drive:" << "    " << e.get_drive() << std::endl;
    std::cout << std::setw(width) << "directory:" << "    " << std::setw(e.get_drive().length()) << "" << e.get_directory() << std::endl;
    std::cout << std::setw(width) << "file:" << "    " << std::setw(e.get_directory_path().length()) << "" << e.get_file() << std::endl;
    std::cout << std::setw(width) << "file name:" << "    " << std::setw(e.get_directory_path().length()) << "" << e.get_file_name() << std::endl;
    std::cout << std::setw(width) << "file extension:" << "    " << std::setw(e.get_directory_path().length() + e.get_file_name().length()) << "" << e.get_file_extension() << std::endl;
#endif /* RECLS_CPP_METHOD_PROPERTY_SUPPORT */

    std::cout << std::setw(width) << "directory parts:" << std::endl;
#ifdef RECLS_CPP_METHOD_PROPERTY_SUPPORT
    directory_parts parts = e.DirectoryParts;
#else /* ? RECLS_CPP_METHOD_PROPERTY_SUPPORT */
    directory_parts parts = e.get_directory_parts();
#endif /* RECLS_CPP_METHOD_PROPERTY_SUPPORT */
    size_t partWidth = width + e.get_drive().length();
    size_t numParts = parts.end() - parts.begin();
    STLSOFT_ASSERT(parts.size() == numParts);
#if 0
    std::cout << "\n" << "num parts:" << numParts << std::endl;
    std::cout << "\n" << "num parts:" << parts.size() << std::endl;

    std::cout << "\n" << "part begin:" << &*parts.begin() << std::endl;
    std::cout << "\n" << "part end:" << &*parts.end() << std::endl;
#endif /* 0 */

    if (numParts > 3)
    {
        (parts.begin() + 3) - 3;
        parts.begin()[3];
    }
    { for (directory_parts::const_iterator i = parts.begin(), e = parts.end(); i != e; ++i)
    {
        std::cout << std::setw(partWidth) << "" << "    " << (*i) << std::endl;
        partWidth += (*i).length();
    }}

#ifdef RECLS_CPP_METHOD_PROPERTY_SUPPORT
    std::cout << std::setw(width) << "Attributes:" << "    " << "0x" << std::setw(8) << std::setfill('0') << std::setbase(16) << static_cast<unsigned>(e.Attributes) << std::setfill(' ') << std::endl;

    std::cout << std::setw(width) << "Size:" << "    " << static_cast<unsigned>(e.Size) << std::endl;
#else /* ? RECLS_CPP_METHOD_PROPERTY_SUPPORT */
    std::cout << std::setw(width) << "attributes:" << "    " << "0x" << std::setw(8) << std::setfill('0') << std::setbase(16) << static_cast<unsigned>(e.get_attributes()) << std::setfill(' ') << std::endl;

    std::cout << std::setw(width) << "size:" << "    " << static_cast<unsigned>(e.get_size()) << std::endl;
#endif /* RECLS_CPP_METHOD_PROPERTY_SUPPORT */

//      std::cout << std::setw(width) << "creation time:" << "    " << e.get_creation_time() << std::endl;
//      std::cout << std::setw(width) << "last access time:" << "    " << e.get_creation_time() << std::endl;
//      std::cout << std::setw(width) << "last status change time:" << "    " << e.get_creation_time() << std::endl;
//      std::cout << std::setw(width) << "modification time:" << "    " << e.get_creation_time() << std::endl;

#ifdef RECLS_CPP_METHOD_PROPERTY_SUPPORT
    std::cout << std::setw(width) << "IsDirectory:" << "    " << (e.IsDirectory ? "true" : "false") << std::endl;
    std::cout << std::setw(width) << "IsReadOnly:" << "    " << (e.IsReadOnly ? "true" : "false") << std::endl;
    std::cout << std::setw(width) << "IsUnc:" << "    " << (e.IsUnc ? "true" : "false") << std::endl;
#else /* ? RECLS_CPP_METHOD_PROPERTY_SUPPORT */
    std::cout << std::setw(width) << "is_directory:" << "    " << (e.is_directory() ? "true" : "false") << std::endl;
    std::cout << std::setw(width) << "is_readonly:" << "    " << (e.is_readonly() ? "true" : "false") << std::endl;
    std::cout << std::setw(width) << "is_unc:" << "    " << (e.is_unc() ? "true" : "false") << std::endl;
#endif /* RECLS_CPP_METHOD_PROPERTY_SUPPORT */

    std::cout << std::endl;
}

/* ////////////////////////////////////////////////////////////////////// */

int main(int argc, char** argv)
{
    int             res;

#if defined(_MSC_VER) && \
    defined(_DEBUG)

    _CrtMemState    memState;
#endif /* _MSC_VER && _MSC_VER */

#if defined(_MSC_VER) && \
    defined(_DEBUG)

    _CrtMemCheckpoint(&memState);
#endif /* _MSC_VER && _MSC_VER */

#if 0
    { for (size_t i = 0; i < 0xffffffff; ++i){} }
#endif /* 0 */

    try
    {
#if defined(_DEBUG) || \
    defined(__SYNSOFT_DBS_DEBUG)

        puts("test.scratch.cpp_api: " __STLSOFT_COMPILER_LABEL_STRING);
#endif /* debug */

        res = main_(argc, argv);
    }
    catch(std::exception& x)
    {
        fprintf(stderr, "Unhandled error: %s\n", x.what());

        res = EXIT_FAILURE;
    }
    catch(...)
    {
        fprintf(stderr, "Unhandled unknown error\n");

        res = EXIT_FAILURE;
    }

#if defined(_MSC_VER) && \
    defined(_DEBUG)

    _CrtMemDumpAllObjectsSince(&memState);
#endif /* _MSC_VER) && _DEBUG */

    return res;
}

/* ///////////////////////////// end of file //////////////////////////// */

