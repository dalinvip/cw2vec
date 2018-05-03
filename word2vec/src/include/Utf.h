// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * utf.h - the utilities for unicode characters.                *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.6                        *
 *                                                              *
 ****************************************************************/

#ifndef _UTILITY_UTF_H
#define _UTILITY_UTF_H

#include <string>
#include <cassert>

/*===============================================================
 *
 * Unicode std::string and character utils
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getUTF8StringLength - get how many characters are in a UTF8 std::string
 *
 *--------------------------------------------------------------*/

inline
unsigned long int getUTF8StringLength(const std::string &s) {
  unsigned long int retval = 0;
  unsigned long int idx = 0;
  while (idx < s.length()) {
    if ((s[idx] & 0x80) == 0) {
      ++idx;
      ++retval;
    } else if ((s[idx] & 0xE0) == 0xC0) {
      idx += 2;
      ++retval;
    } else if ((s[idx] & 0xF0) == 0xE0) {
      idx += 3;
      ++retval;
    } else {
      //std::cerr << "Warning: " << "in utf.h getUTF8StringLength: std::string '" << s << "' not encoded in unicode utf-8" << std::endl;
      if(s.length()-  idx < 4)
      {
        return retval+1;
      }
      else
      {
        idx += 4;
        ++retval;
      }
    }
  }
  if (idx != s.length()) {
    //std::cerr << "Warning: " << "in utf.h getUTF8StringLength: std::string '" << s << "' not encoded in unicode utf-8" << std::endl;
    return retval+1;
  }
  return retval;
}

/*----------------------------------------------------------------
 *
 * getCharactersFromUTF8String - get the characters from 
 *                               utf std::string. The characters from 
 *                               this std::string are appended
 *                               to a given sentence. 
 *
 *----------------------------------------------------------------*/

inline int getCharactersFromUTF8String(const std::string &s, std::vector<string>& sentence) {
  sentence.clear();
  unsigned long int idx = 0;
  unsigned long int len = 0;
  while (idx < s.length()) {
    if ((s[idx] & 0x80) == 0) {
      sentence.push_back(s.substr(idx, 1));
      ++len;
      ++idx;
    } else if ((s[idx] & 0xE0) == 0xC0) {
      sentence.push_back(s.substr(idx, 2));
      ++len;
      idx += 2;
    } else if ((s[idx] & 0xF0) == 0xE0) {
      sentence.push_back(s.substr(idx, 3));
      ++len;
      idx += 3;
    } else {
      if(s.length()-  idx < 4)
      {
        sentence.push_back(s.substr(idx));
        ++len;
        idx = s.length();
      }
      else
      {
        sentence.push_back(s.substr(idx, 4));
        ++len;
        idx += 4;
      }
    }
  }
  if (idx != s.length()) {
    //std::cerr << "Warning: " << "in utf.h getCharactersFromUTF8String: std::string '" << s << "' not encoded in utf-8" << std::endl;
    return len+1;
  }

  return len;
}

/*----------------------------------------------------------------
 *
 * getFirstCharFromUTF8String - get the first character from 
 *                              utf std::string. 
 *
 *----------------------------------------------------------------*/

inline std::string getFirstCharFromUTF8String(const std::string &s) {
  if (s == "")
    return "";
  if ((s[0] & 0x80) == 0) {
    return s.substr(0, 1);
  } else if ((s[0] & 0xE0) == 0xC0) {
    assert(s.length() >= 2);
    return s.substr(0, 2);
  } else if ((s[0] & 0xF0) == 0xE0) {
    assert(s.length() >= 3);
    return s.substr(0, 3);
  } else {
    //std::cerr << "Warning: " << "in utf.h getFirstCharFromUTF8String: std::string '" << s << "' not encoded in unicode utf-8" << std::endl;
    if(s.length() < 4)
    {
      return s;
    }
    else
    {
      return s.substr(0, 4);
    }
  }
}

/*----------------------------------------------------------------
 *
 * getLastCharFromUTF8String - get the last character from 
 *                             utf std::string.  
 *
 *----------------------------------------------------------------*/

inline std::string getLastCharFromUTF8String(const std::string &s) {
  if (s == "")
    return "";
  unsigned long int idx = 0;
  std::string retval;
  while (idx < s.length()) {
    if ((s[idx] & 0x80) == 0) {
      retval = s.substr(idx, 1);
      ++idx;
    } else if ((s[idx] & 0xE0) == 0xC0) {
      retval = s.substr(idx, 2);
      idx += 2;
    } else if ((s[idx] & 0xF0) == 0xE0) {
      retval = s.substr(idx, 3);
      idx += 3;
    } else {
      //std::cerr << "Warning: " << "in utf.h getLastCharFromUTF8String: std::string '" << s << "' not encoded in unicode utf-8" << std::endl;
      if(s.length()-  idx < 4)
      {
        return s;
      }
      else
      {
        retval = s.substr(idx, 4);
        idx += 4;
      }
      return s;
    }
  }
  if (idx != s.length()) {
    //std::cerr << "Warning: " << "in utf.h getLastCharFromUTF8String: std::string '" << s << "' not encoded in unicode utf-8" << std::endl;
    return s;
  }
  return retval;
}

/*----------------------------------------------------------------
 *
 * isOneUTF8Character - whether a std::string is one utf8 character
 *
 *----------------------------------------------------------------*/

inline bool isOneUTF8Character(const std::string &s) {
  if (s == "")
    return false; // is no utf character
  if (s.size() > 3)
    return false; // is more than one utf character
  if ((s[0] & 0x80) == 0) {
    return s.size() == 1;
  } else if ((s[0] & 0xE0) == 0xC0) {
    return s.size() == 2;
  } else if ((s[0] & 0xF0) == 0xE0) {
    return s.size() == 3;
  }
}

inline std::string getUTF8CharType(const std::string &s) {
  std::string digit = "0123456789";
  std::string eng = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::string defaultType = "default";
  if (s.length() <= 2) {
    if (digit.find(s) != -1) {
      defaultType = "digit";
    } else if (eng.find(s) != -1) {
      defaultType = "eng";
    }
  } else {
    defaultType = "unitype";
  }

  return defaultType;
}

inline std::string wordtype(const std::string &s) {
  std::vector<string> chars;
  getCharactersFromUTF8String(s, chars);
  std::string type = "";
  for (int i = 0; i < chars.size(); i++) {
    if (chars[i].length() > 1) {
      type = type + "u";
    } else if (isdigit(chars[i][0])) {
      type = type + "d";
    } else if (isalpha(chars[i][0])) {
      if (islower(chars[i][0]))
        type = type + "e";
      else
        type = type + "E";
    }
  }
  return type;
}

inline std::string normalize_to_lowerwithdigit(const std::string& s)
{
  std::vector<string> chars;
  getCharactersFromUTF8String(s, chars);
  std::string lowcase = "";
  for (int i = 0; i < chars.size(); i++) {
    if (chars[i].length() > 1) {
      lowcase = lowcase + chars[i];
    } else if (isdigit(chars[i][0])) {
      lowcase = lowcase + "0";
    } else if (isalpha(chars[i][0])) {
      if (islower(chars[i][0]))
      {
        lowcase = lowcase + chars[i][0];
      }
      else
      {
        char temp = chars[i][0] + 'a'-'A';
        lowcase = lowcase + temp;
      }
    }
    else
    {
      lowcase = lowcase + chars[i];
    }
  }
  return lowcase;
}


/*----------------------------------------------------------------
 *
 * getCharactersFromUTF8String - get the characters from
 *                               utf std::string. The characters from
 *                               this std::string are appended
 *                               to a given sentence.
 *
 *----------------------------------------------------------------*/

inline int getCharactersFromString(const std::string &s, std::vector<string>& sentence) {
  sentence.clear();
  unsigned long int idx = 0;
  unsigned long int len = 0;
  while (idx < s.length()) {
    if ((s[idx] & 0x80) == 0) {
      sentence.push_back(s.substr(idx, 1));
      ++len;
      ++idx;
    } else if ((s[idx] & 0xE0) == 0xC0) {
      sentence.push_back(s.substr(idx, 2));
      ++len;
      idx += 2;
    } else if ((s[idx] & 0xF0) == 0xE0) {
      sentence.push_back(s.substr(idx, 3));
      ++len;
      idx += 3;
    } else {
      //std::cerr << "Warning: " << "in utf.h getCharactersFromUTF8String: std::string '" << s << "' not encoded in unicode utf-8" << std::endl;
      if(s.length()-  idx < 4)
      {
        sentence.push_back(s.substr(idx));
        ++len;
        idx = s.length();
      }
      else
      {
        sentence.push_back(s.substr(idx, 4));
        ++len;
        idx += 4;
      }
    }
  }

  if (idx != s.length()) {
    //std::cerr << "Warning: " << "in utf.h getCharactersFromUTF8String: std::string '" << s << "' not encoded in utf-8" << std::endl;
    return len+1;
  }

  return len;
}

#endif
