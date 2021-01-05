/*
 * Copyright (c) 2020 Alex Spataru <https://github.com/alex-spataru>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Translator.h"

/**
 * Constructor function
 */
Translator::Translator()
{
   m_language = systemLanguage();
}

/**
 * Returns the current language ID, which corresponds to the indexes of the languages returned
 * by the \c availableLanguages() function.
 */
int Translator::language() const
{
   return m_language;
}

/**
 * Returns the appropiate language ID based on the current locale settings of the host's operating
 * system.
 */
int Translator::systemLanguage() const
{
   int lang;
   switch (QLocale::system().language())
   {
      case QLocale::English:
         lang = 0;
         break;
      case QLocale::Spanish:
         lang = 1;
         break;
      default:
         lang = 0;
         break;
   }

   return lang;
}

/**
 * Returns an empty string, this function should be used with QML to automatically update
 * all the strings without the need of restarting the application. For example, the following
 * QML code:
 *
 *      text: qsTr("Text") + CppTranslator.dummyString
 *
 * Will force the QML interface to update the value of @a text when the language is changed.
 */
QString Translator::dummyString() const
{
   return "";
}

/**
 * Returns the welcome text displayed on the console
 */
QString Translator::welcomeConsoleText() const
{
   QString lang = "EN";
   if (language() == 1)
      lang = "ES";

   QString text = QObject::tr("Failed to load welcome text :(");
   QFile file(":/messages/Welcome_" + lang + ".txt");
   if (file.open(QFile::ReadOnly))
   {
      text = QString::fromUtf8(file.readAll());
      file.close();
   }

   return text;
}

/**
 * Returns a list with the available translation languages.
 */
QStringList Translator::availableLanguages() const
{
   return QStringList { "English", "Español" };
}

/**
 * Changes the language of the application and emits the signals appropiate to reload every
 * string that uses the Qt translator system.
 *
 * @param language language ID based on the indexes of the \a availableLanguages() function
 */
void Translator::setLanguage(const int language)
{
   QString langName;
   QLocale locale;
   switch (language)
   {
      case 0:
         langName = "en";
         locale = QLocale(QLocale::English);
         break;
      case 1:
         langName = "es";
         locale = QLocale(QLocale::Spanish);
         break;
      default:
         langName = "en";
         locale = QLocale(QLocale::English);
         break;
   }

   m_language = language;
   setLanguage(locale, langName);
}

/**
 * Changes the language of the application and emits the signals neccesary to reload every
 * string that uses the Qt translator system.
 *
 * @param locale    user-set locale
 * @param language  name of the *.qm file to load from the "translations" directory inside the
 *                  application's resources
 */
void Translator::setLanguage(const QLocale &locale, const QString &language)
{
   qApp->removeTranslator(&m_translator);
   m_translator.load(locale, ":/translations/" + language + ".qm");
   qApp->installTranslator(&m_translator);
   emit languageChanged();
}
