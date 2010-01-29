/*!
 * \file   ThemeManager.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 f�v 2008
 */

#ifndef _LIB_TFEL_THEMEMANAGER_HXX_
#define _LIB_TFEL_THEMEMANAGER_HXX_ 

#include<string>
#include<map>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"TFEL/Graphics/GraphTheme.hxx"

namespace tfel
{

  namespace graphics
  {

    struct ThemeProxy
    {
      typedef tfel::utilities::SmartPtr<GraphTheme> ThemePtr;
      virtual const std::string
      getName(void) const = 0;
      virtual const ThemePtr
      getTheme(void) const = 0;
      virtual ~ThemeProxy();
    }; // end of struct ThemeProxy

    template<typename T>
    struct StandardThemeProxy
      : public ThemeProxy
    {
      StandardThemeProxy(const std::string&,
			 const bool = true);
      const std::string
      getName(void) const;
      const ThemeProxy::ThemePtr
      getTheme(void) const;
      ~StandardThemeProxy();
    private:
      const std::string name;
    }; // end of struct StandardThemeProxy

    struct ThemeManager
    {
      typedef tfel::utilities::SmartPtr<GraphTheme> ThemePtr;

      static ThemeManager&
      getThemeManager();

      void
      loadLibrary(const std::string&);
      
      const ThemePtr
      getTheme(const std::string&);

      void
      addTheme(const ThemeProxy * const);

      const std::vector<std::string>
      getAvailableThemesNames(void) const;

    private:

      ThemeManager();

      ThemeManager(const ThemeManager&);

      ThemeManager&
      operator=(const ThemeManager&);

      ~ThemeManager();

      std::map<std::string,tfel::utilities::SmartPtr<const ThemeProxy> > proxies;

    }; // end of struct ThemeManager

  } // end of namespace graphics

} // end of namespace tfel

#include"TFEL/Graphics/ThemeManager.ixx"

#endif /* _LIB_TFEL_THEMEMANAGER_HXX */
