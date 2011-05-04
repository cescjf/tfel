/*! 
 * \file  TestDocMain.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#include<string>
#include<vector>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<iostream>
#include<iterator>
#include<stdexcept>
#include<algorithm>
#include<stdexcept>
#include<cstdlib>
#include<cerrno>
#include<cstring>

#include <sys/param.h>

#include"TFEL/System/System.hxx"
#include"TFEL/System/RecursiveFind.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

#include"TFEL/Utilities/Global.hxx"
#include"TFEL/Utilities/LaTeXConvertion.hxx"
#include"TFEL/Utilities/TestDocumentation.hxx"
#include"TFEL/Utilities/TestDocParser.hxx"
#include"TFEL/Utilities/ConfigParser.hxx"
#include"TFEL/Utilities/TestDocMain.hxx"

namespace tfel
{

  namespace utilities
  {
   
    static std::string
    getSectionHeaderTranslation(const std::string& s,
				const std::string& l)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(l=="french"){
	return "Test de la cat�gorie \\og~"+getCategory(s,l)+"~\\fg{}";
      }
      if(l!="english"){
	cerr << TerminalColors::Red;
	cerr << "getSectionHeaderTranslation : unsupported language'"+l
	     << "', using default" << endl;
	cerr << TerminalColors::Reset;
      }
      return s;
    }

    static std::string
    getBabelPackage(const std::string& l)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(l!="english"){
	cerr << TerminalColors::Red;
	cerr << "getBabelPackage : unsupported language '" << l << 
	  "', using default" << endl;
	cerr << TerminalColors::Reset;
      }
      return "anglais";
    }

    static void
    printLaTeXFile(std::ostream& log,
		   const std::map<std::string,std::vector<TestDocumentation> >& tests,
		   const std::string& l)
    {
      using namespace std;
      map<string,vector<TestDocumentation> >::const_iterator p;
      vector<TestDocumentation>::const_iterator p2;
      if(l!="french"){
	log << "\\documentclass[rectoverso,index,pleiades,pstricks,leqno,anti,latin1,projet,"
	    << getBabelPackage(l) << "]{note_technique_2010}" << endl;
      } else {
	log << "\\documentclass[rectoverso,index,pleiades,pstricks,leqno,anti,latin1,projet]{note_technique_2010}" << endl;
      }
      log << endl;
      log << "\\input{LSC}" << endl;
      log << "\\auteurs{T.~Helfer}" << endl;
      log << "\\redacteur{T.~Helfer}" << endl;
      log << "\\titre{Guide des cas tests celaeno}" << endl;
      log << "\\date{2011}" << endl;
      log << endl;
      log << "\\usepackage{multind}" << endl;
      log << endl;
      log << "\\newcommand{\\Frac}[2]{\\displaystyle\\frac{\\displaystyle #1}{\\displaystyle #2}}" << endl;
      log << "\\newcommand{\\paren}[1]{\\ensuremath\\left(#1\\right)}" << endl;
      log << "\\newcommand{\\tenseur}[1]{\\ensuremath\\underline{#1}}" << endl;
      log << "\\newcommand{\\tenseurq}[1]{\\ensuremath\\underline{\\underline{#1}}}" << endl;
      log << "\\newcommand{\\nom}[1]{\\textsc{#1}}" << endl;
      log << endl;
      log << "% one column index" << endl;
      log << "\\makeatletter" << endl;
      log << "\\def\\printindex#1#2{\\section*{#2}" << endl;
      log << "\\addcontentsline{toc}{section}{#2}" << endl;
      log << "\\@input{#1.ind}}"  << endl;
      log << "\\makeatother" << endl;
      log << endl;

      log << "\\makeindex{general}" << endl;
      log << "\\makeindex{models}" << endl;
      log << endl;
      log << "\\begin{document}"<< endl;
      log << endl;
      for(p=tests.begin();p!=tests.end();++p){
	log << endl;
	log << "\\clearpage" << endl;
	log << "\\newpage" << endl;
	log << "\\section{" << getSectionHeaderTranslation(p->first,l) << "}" << endl;
	log << endl; 
	for(p2=p->second.begin();p2!=p->second.end();++p2){
	  p2->writeLaTexDescription(log,l);
	}
      }
      log << "\\clearpage" << endl;
      log << "\\newpage" << endl;
      log << "\\printindex{general}{" << LaTeXConvertion::capitalize(getTranslation("general index",l)) << "}" << endl  << endl;
      log << "\\clearpage" << endl;
      log << "\\newpage" << endl;
      log << "\\printindex{models}{" << LaTeXConvertion::capitalize(getTranslation("models index",l)) << "}" << endl << endl;
      log << "\\end{document}"<< endl;
    }

    static void
    declareKeys(const std::string& f)
    {
      ConfigParser parser;
      parser.execute(getKeysMap(),f);
    }

    static void
    declareCategories(const std::string& f)
    {
      ConfigParser parser;
      parser.execute(getCategoriesMap(),f);
    }

    static void
    declareTranslations(const std::string& f)
    {
      ConfigParser parser;
      parser.execute(getTranslationsMap(),f);
    }

    TestDocMain::TestDocMain(const int argc,
			     const char*const* argv)
      : ArgumentParserBase<TestDocMain>(argc,argv)
    {
      using namespace std;
      using namespace tfel::utilities;
      this->registerArgumentCallBacks();
      this->parseArguments();
      if(this->outputFile.empty()){
	cerr << "TestDocMain : no output file specified\n";
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      this->output.open(this->outputFile.c_str());
      if(!this->output){
	string msg("TestDocMain : can't open output file '");
	msg += this->outputFile;
	msg += '\'';
	throw(runtime_error(msg));
      }
      if(!this->logFile.empty()){
	this->logptr = shared_ptr<ostream>(new ofstream(argv[2]));
	if(!(*(this->logptr))){
	  string msg("TestDocMain : can't open log file '");
	  msg += this->logFile;
	  msg += '\'';
	  throw(runtime_error(msg));
	}
	this->log = this->logptr.get();
      } else{ 
	this->log = &cerr;
      }
      if(this->lang.empty()){
	lang = "english";
      }
    } // end of TestDocMain::TestDocMain

    void
    TestDocMain::treatUnknownArgument(void)
    {
      using namespace std;
      if((*(this->currentArgument))[0]=='-'){
	ArgumentParserBase<TestDocMain>::treatUnknownArgument();
	cerr << "TestDocMain : unsupported option '" << *(this->currentArgument) << "'\n";
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      if(this->outputFile.empty()){
	this->outputFile = *(this->currentArgument);
      } else {
	cerr << "TestDocMain : output file already specified" << endl;
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
    } // end of TestDocMain::treatUnknownArgument

    void 
    TestDocMain::registerArgumentCallBacks(void)
    {
      this->registerNewCallBack("--lang",&TestDocMain::treatSrc,
				"specify output language (french,english)",true);
      this->registerNewCallBack("--src",&TestDocMain::treatSrc,
				"specify root of sources",true);
      this->registerNewCallBack("--log",&TestDocMain::treatLogFile,
				"specify a log file",true);
      this->registerNewCallBack("--keys",&TestDocMain::treatKeyFile,
				"specify a key file",true);
      this->registerNewCallBack("--categories",&TestDocMain::treatCategoryFile,
				"specify a category file",true);
      this->registerNewCallBack("--translations",&TestDocMain::treatTranslationFile,
				"specify a translation file",true);
    } // end of TestDocMain::registerArgumentCallBacks

    void
    TestDocMain::treatLogFile(void)
    {
      using namespace std;
      if(!this->logFile.empty()){
	cerr << "TestDocMain : log file already specified" << endl;
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      this->logFile = this->currentArgument->getOption();
      if(this->logFile.empty()){
	string msg("TestDocMain::treatlogFile : ");
	msg += "no option given to the --logFile argument";
	throw(runtime_error(msg));
      }
    } // end of TestDocMain::treatLogFile

    void
    TestDocMain::treatSrc(void)
    {
      using namespace std;
      if(!this->srcdir.empty()){
	cerr << "TestDocMain : srcdir file already specified" << endl;
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      this->srcdir = this->currentArgument->getOption();
      if(this->srcdir.empty()){
	string msg("TestDocMain::treatSrc : ");
	msg += "no option given to the --src argument";
	throw(runtime_error(msg));
      }
    } // end of TestDocMain::treatSrc

    void
    TestDocMain::treatLang(void)
    {
      using namespace std;
      if(!this->lang.empty()){
	cerr << "TestDocMain : lang file already specified" << endl;
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      this->lang = this->currentArgument->getOption();
      if(this->lang.empty()){
	string msg("TestDocMain::treatLang : ");
	msg += "no option given to the --src argument";
	throw(runtime_error(msg));
      }
    } // end of TestDocMain::treatLang

    void
    TestDocMain::treatKeyFile(void)
    {
      using namespace std;
      const string& f = this->currentArgument->getOption();
      declareKeys(f);
    } // end of TestDocMain::treatKeyFile

    void
    TestDocMain::treatCategoryFile(void)
    {
      using namespace std;
      const string& f = this->currentArgument->getOption();
      declareCategories(f);
    } // end of TestDocMain::treatCategoryFile

    void
    TestDocMain::treatTranslationFile(void)
    {
      using namespace std;
      const string& f = this->currentArgument->getOption();
      declareTranslations(f);
    } // end of TestDocMain::treatTranslationFile

    std::string 
    TestDocMain::getVersionDescription(void) const
    {
      using namespace std;
      return "1.0";
    }
    
    std::string 
    TestDocMain::getUsageDescription(void) const
    {
      using namespace std;
      string usage("Usage : tfel-doc [options] output");
      return usage;
    }

    int
    TestDocMain::execute(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::system;
      char path[MAXPATHLEN];
      char cpath[MAXPATHLEN];
      map<string,vector<string> >::const_iterator p;
      vector<string>::const_iterator p2;
      map<string,vector<TestDocumentation> > tests;

      if(!this->srcdir.empty()){
	if(chdir(this->srcdir.c_str())==-1){
	  *(this->log) << "can't move to directory " << this->srcdir << endl;
	  exit(EXIT_FAILURE);
	}
      }

      map<string,vector<string> > files;
      recursiveFind(files,".*\\.testdoc",".");

      if(realpath(".",cpath)==0){
	*(this->log) << "main : can't get real path of current directory, aborting\n";
	exit(EXIT_FAILURE);
      }
      
      for(p=files.begin();p!=files.end();++p){
	if(realpath(p->first.c_str(),path)==0){
	  *(this->log) << "entering directory " << p->first << endl;
	} else {
	  *(this->log) << "entering directory " << path << endl;
	} 
	for(p2=p->second.begin();p2!=p->second.end();++p2){
	  string name = p->first+'/'+*p2;
	  try{
	    TestDocParser parser(name);
	    parser.execute(tests);
	  }
	  catch(std::exception& e){
	    *(this->log) << TerminalColors::Reset;
	    *(this->log) << "treatment of file '"+*p2+"' failed : "
			 << e.what() << endl;
	    *(this->log) << TerminalColors::Reset;
	  }
	}
      }
      printLaTeXFile(this->output,tests,this->lang);
      map<string,vector<TestDocumentation> >::const_iterator p3;
      map<string,vector<TestDocumentation> >::size_type count = 0u;
      for(p3=tests.begin();p3!=tests.end();++p3){
	count += p3->second.size();
      }
      cout << count << " tests treated in " <<  tests.size() << " categories" << endl;
      return EXIT_SUCCESS;
    }

    TestDocMain::~TestDocMain()
    {} // end of TestDocMain::~TestDocMain()

  } // end of namespace utilities
  
} // end of namespace tfel