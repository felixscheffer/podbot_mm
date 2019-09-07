// The purpose of this program is to be called in the "Pre-Build Step" state of
// your project compilation so as to generate an updated resource file featuring
// an automatically updated version number, along with several other optional
// parameters. It is particularly useful with IDEs where no resource editors are
// available, such as Borland or MingW32. It can also be used with MSVC to always
// keep your executable version numbers up to date.
// 
// The version number string and DWORD are generated automatically by the program
// according to the YYYYMMDD convention (year - month - day), as in "20040228".
//
// makeres.cpp

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>


int myfputs (FILE *fp, const char *fmt, ...)
{
   // this function sends a message string to a file. It's basically a wrapper for fputs() which
   // allows me to pass formatted strings, printf style, instead of building them myself.

   va_list argptr;
   static char string[1024];

   if (fp == NULL)
      return (1); // reliability check

   // concatenate all the arguments in one string
   va_start (argptr, fmt);
   vsprintf (string, fmt, argptr);
   va_end (argptr);

   fputs (string, fp); // send the concatenated string to the file

   return (0); // and return with a "no error" signal
}


const char *MakeVersion (void)
{
   // this function builds the version number string and the welcome message string out of it.
   // The version number string is a 8 digit string describing the date at which the program was
   // compiled, in which the digits are arranged under the form "YYYYMMDD" (year, month, day).
   // This order allow earlier dates to be represented by a smaller 8-digit number and later
   // dates to be represented by greater ones. We use the __DATE__ standard C macro to get the
   // program compile date string ; this string being under the form "Mmm DD, YYYY" such as in
   // "Mar 4, 2003", we need to process it in order to convert it into our own YYYYMMDD format.

   const char *compile_date = __DATE__;
   static char version[64];
   char year[5], month[3], day[3], temp[8];

   // get the year under the form YYYY
   strncpy (temp, compile_date + 7, 4);
   temp[4] = 0; // terminate the string
   sprintf (year, "%04d", atoi (temp)); // and format the 4 digits

   // translate the month string under the form MM
   if (strncmp ("Jan", compile_date, 3) == 0)
      strcpy (month, "01");
   else if (strncmp ("Feb", compile_date, 3) == 0)
      strcpy (month, "02");
   else if (strncmp ("Mar", compile_date, 3) == 0)
      strcpy (month, "03");
   else if (strncmp ("Apr", compile_date, 3) == 0)
      strcpy (month, "04");
   else if (strncmp ("May", compile_date, 3) == 0)
      strcpy (month, "05");
   else if (strncmp ("Jun", compile_date, 3) == 0)
      strcpy (month, "06");
   else if (strncmp ("Jul", compile_date, 3) == 0)
      strcpy (month, "07");
   else if (strncmp ("Aug", compile_date, 3) == 0)
      strcpy (month, "08");
   else if (strncmp ("Sep", compile_date, 3) == 0)
      strcpy (month, "09");
   else if (strncmp ("Oct", compile_date, 3) == 0)
      strcpy (month, "10");
   else if (strncmp ("Nov", compile_date, 3) == 0)
      strcpy (month, "11");
   else if (strncmp ("Dec", compile_date, 3) == 0)
      strcpy (month, "12");
   else
      strcpy (month, "??"); // unable to understand the month string, WTH ???
   month[2] = 0; // terminate the string

   // get the day under the form DD
   strncpy (temp, compile_date + 4, 2);
   temp[2] = 0; // terminate the string
   sprintf (day, "%02d", atoi (temp)); // and format the 2 digits

   // build the version string and the welcome text string
   sprintf (version, "%s%s%s", year, month, day);

   return (version); // finished
}


int main (int argc, char **argv)
{
   // this function creates a minimal RC file that will be used to build a MSVC project,
   // featuring the appropriate version number and optional strings passed as arguments.

   FILE *fp = NULL;
   char filename[256] = "";
   char version_versionstring[32], temp_string[32];
   int version_year, version_month, version_day, version_release, index;
   char version_comment[256] = "";
   char version_companyname[256] = "";
   char version_filedescription[256] = "";
   char version_internalname[256] = "";
   char version_legalcopyright[256] = "";
   char version_legaltrademarks[256] = "";
   char version_originalfilename[256] = "";
   char version_privatebuild[256] = "";
   char version_productname[256] = "";
   char version_specialbuild[256] = "";

   // no argument specified ?
   if (argc == 1)
      goto PrintUsageAndExit; // then print a little help blurb and terminate the program

   // parse all arguments
   for (index = 1; index < argc; index++)
   {
      if (argv[index][0] != '-')
         strcpy (filename, argv[index]); // if not beginning by a dash, it's the filename

      else if (strnicmp ("-t", argv[index], 2) == 0)
         strcpy (version_comment, argv[index] + 2); // comment
      else if (strnicmp ("-c", argv[index], 2) == 0)
         strcpy (version_companyname, argv[index] + 2); // company name
      else if (strnicmp ("-d", argv[index], 2) == 0)
         strcpy (version_filedescription, argv[index] + 2); // file description
      else if (strnicmp ("-i", argv[index], 2) == 0)
         strcpy (version_internalname, argv[index] + 2); // internal name
      else if (strnicmp ("-l", argv[index], 2) == 0)
         strcpy (version_legalcopyright, argv[index] + 2); // legal copyright
      else if (strnicmp ("-m", argv[index], 2) == 0)
         strcpy (version_legaltrademarks, argv[index] + 2); // legal trademarks
      else if (strnicmp ("-o", argv[index], 2) == 0)
         strcpy (version_originalfilename, argv[index] + 2); // original filename
      else if (strnicmp ("-p", argv[index], 2) == 0)
         strcpy (version_privatebuild, argv[index] + 2); // private build
      else if (strnicmp ("-n", argv[index], 2) == 0)
         strcpy (version_productname, argv[index] + 2); // product name
      else if (strnicmp ("-s", argv[index], 2) == 0)
         strcpy (version_specialbuild, argv[index] + 2); // special build

      else
         goto PrintUsageAndExit; // invalid parameter
   }

   // no output file specified ?
   if (filename[0] == 0)
   {
      printf ("%s: no output filename\n", argv[0]);
      goto PrintUsageAndExit; // then exit with an error condition
   }

   // open version.rc file for writing, and squash all that's in it
   fp = fopen (filename, "w");
   if (fp == NULL)
   {
      printf ("Error opening resource file \"%s\" for writing\n", filename);
      goto PrintUsageAndExit; // exit with an error condition if the file couldn't be open
   }

   // make the version string
   sprintf (version_versionstring, MakeVersion ());

   // make the version year, month, day and release
   memset (temp_string, 0, sizeof (temp_string));
   strncpy (temp_string, version_versionstring + 0, 4);
   version_year = atoi (temp_string);
   memset (temp_string, 0, sizeof (temp_string));
   strncpy (temp_string, version_versionstring + 4, 2);
   version_month = atoi (temp_string);
   memset (temp_string, 0, sizeof (temp_string));
   strncpy (temp_string, version_versionstring + 6, 2);
   version_day = atoi (temp_string);
   memset (temp_string, 0, sizeof (temp_string));
   strcpy (temp_string, version_versionstring + 8);
   version_release = atoi (temp_string);

   // now generate the RC file
   myfputs (fp, "// Auto-generated, so don't bother.\n");
   myfputs (fp, "\n");
   myfputs (fp, "#include <winver.h>\n");
   myfputs (fp, "\n");
   myfputs (fp, "VS_VERSION_INFO VERSIONINFO\n");
   myfputs (fp, "FILEVERSION %d,%d,%d,%d\n", version_year, version_month, version_day, version_release);
   myfputs (fp, "PRODUCTVERSION %d,%d,%d,%d\n", version_year, version_month, version_day, version_release);
   myfputs (fp, "FILEFLAGSMASK 0x3fL\n");
   myfputs (fp, "#ifdef _DEBUG\n");
   myfputs (fp, "FILEFLAGS VS_FF_DEBUG\n");
   myfputs (fp, "#else\n");
   myfputs (fp, "FILEFLAGS 0x0L\n");
   myfputs (fp, "#endif\n");
   myfputs (fp, "FILEOS VOS__WINDOWS32\n");
   myfputs (fp, "FILETYPE VFT_DLL\n");
   myfputs (fp, "FILESUBTYPE VFT2_UNKNOWN\n");
   myfputs (fp, "BEGIN\n");
   myfputs (fp, "   BLOCK \"StringFileInfo\"\n");
   myfputs (fp, "   BEGIN\n");
   myfputs (fp, "      BLOCK \"040904b0\"\n");
   myfputs (fp, "      BEGIN\n");
   myfputs (fp, "         VALUE \"Comments\", \"%s\\0\"\n", version_comment);
   myfputs (fp, "         VALUE \"CompanyName\", \"%s\\0\"\n", version_companyname);
   myfputs (fp, "         VALUE \"FileDescription\", \"%s\\0\"\n", version_filedescription);
   myfputs (fp, "         VALUE \"FileVersion\", \"%s\\0\"\n", version_versionstring);
   myfputs (fp, "         VALUE \"InternalName\", \"%s\\0\"\n", version_internalname);
   myfputs (fp, "         VALUE \"LegalCopyright\", \"%s\\0\"\n", version_legalcopyright);
   myfputs (fp, "         VALUE \"LegalTrademarks\", \"%s\\0\"\n", version_legaltrademarks);
   myfputs (fp, "         VALUE \"OriginalFilename\", \"%s\\0\"\n", version_originalfilename);
   myfputs (fp, "         VALUE \"PrivateBuild\", \"%s\\0\"\n", version_privatebuild);
   myfputs (fp, "         VALUE \"ProductName\", \"%s\\0\"\n", version_productname);
   myfputs (fp, "         VALUE \"ProductVersion\", \"%s\\0\"\n", version_versionstring);
   myfputs (fp, "         VALUE \"SpecialBuild\", \"%s\\0\"\n", version_specialbuild);
   myfputs (fp, "      END\n");
   myfputs (fp, "   END\n");
   myfputs (fp, "   BLOCK \"VarFileInfo\"\n");
   myfputs (fp, "   BEGIN\n");
   myfputs (fp, "      VALUE \"Translation\", 0x409, 1200\n");
   myfputs (fp, "   END\n");
   myfputs (fp, "END\n");

   fclose (fp); // finished, close the file

   printf ("Resource file generated successfully.\n"); // reassure the end user...
   return (0); // ...and go away!

PrintUsageAndExit:
   // this code is only reached when something went wrong
   printf ("%s - Project resource file generator\n", argv[0]);
   printf ("\tby Pierre-Marie Baty <pm@bots-united.com>\n");
   printf ("\n");
   printf ("Usage: %s\t[-t\"Comment\"] [-c\"Company Name\"] [-d\"File Description\"]\n", argv[0]);
   printf ("\t[-i\"Internal Name\"] [-l\"Legal Copyright\"] [-m\"Legal Trademarks\"]\n");
   printf ("\t[-o\"Original Filename\"] [-p\"Private Build\"] [-n\"Product Name\"]\n");
   printf ("\t[-s\"Special Build\"] filename.rc\n");
   printf ("\n");
   printf ("\t\tGenerates a resource file using the specified parameters.\n");
   printf ("\n");
   printf ("\t%s {-h|-?|/h|/?|--help|/help}\n", argv[0]);
   printf ("\n");
   printf ("\t\tPrints this help text and program usage.\n");
   printf ("\n");
   printf ("The purpose of this program is to be called in the \"Pre-Build Step\" state of\n");
   printf ("your project compilation so as to generate an updated resource file featuring\n");
   printf ("an automatically updated version number, along with several other optional\n");
   printf ("parameters. It is particularly useful with IDEs where no resource editors are\n");
   printf ("available, such as Borland or MingW32. It can also be used with MSVC to always\n");
   printf ("keep your executable version numbers up to date.\n");
   printf ("\n");
   printf ("The version number string and DWORD are generated automatically by the program\n");
   printf ("according to the YYYYMMDD convention (year - month - day), as in \"20040228\".\n");

   return (1); // exit with an error condition
}
