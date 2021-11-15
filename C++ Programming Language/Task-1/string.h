#ifndef __HEADER_STRING
#define __HEADER_STRING

#include <cstring>
#include <iostream>

class String {
      public:
            String ();
            String (char value);
            String (const char* cstring);
            String (const String& other);
            String (size_t n, char c);
            
            ~String ();

            String& operator= (const String& other);

            char& operator[] (size_t index);
            const char& operator[] (size_t index) const;

            size_t length () const;

            void push_back (char value);
            char pop_back ();

            char& front ();
            char& back ();

            const char& front () const;
            const char& back () const;
            
            String& operator+= (char value);
            String& operator+= (const String& other);
            String& operator+= (const char* cstring);

            const String operator+ (char value) const;
            const String operator+ (const String& other) const;

            friend std::ostream& operator<< (std::ostream& stream, const String& s);
            friend std::istream& operator>> (std::istream& stream, String& s);
            
            bool operator== (const String& other) const;
            bool operator== (const char* cstring) const;
            bool operator== (char c) const;

            friend bool operator== (const char *cstring, const String& s);
            friend bool operator== (char c, const String& s);
            
            String substr (size_t start, size_t count) const;

            size_t find (const String& other) const;
            size_t rfind (const String& other) const;

            bool empty () const;

            void clear ();

      private:
            String (size_t n);

            char* __string_array;
            size_t __string_size;
            size_t __allocated_size;

            void __increase ();
            void __decrease ();

            void __resize (size_t new_size);
};

String::String () { // OK
      __allocated_size = 1;
      __string_size = 0;

      __string_array = new char[__allocated_size];
}

String::String (char value) { // OK
      __allocated_size = 2;
      __string_size = 1;

      __string_array = new char[__allocated_size];
      __string_array[0] = value;
}

String::String (const char* cstring) { // OK

      __string_size = strlen (cstring);
      __string_array = new char[__string_size];

      __allocated_size = __string_size;

      strncpy (__string_array, cstring, __string_size);
}

String::String (const String& other) { // OK
      
      __string_size = other.__string_size;
      __allocated_size = other.__allocated_size;

      __string_array = new char[__allocated_size];

      strncpy (__string_array, other.__string_array, __string_size);
}

String::String (size_t n) { // OK

      __string_size = 0;
      __allocated_size = n;

      __string_array = new char[__allocated_size];
}

String::String (size_t n, char c) { // OK

      __string_size = n;
      __allocated_size = n;

      __string_array = new char[__allocated_size];
      
      memset (__string_array, c, __string_size);
}

String::~String () { // OK

      delete [] __string_array;
}


String& String::operator= (const String& other) {

      if (&other == this) {
            return *this;
      }

      __string_size = other.__string_size;
      __allocated_size = other.__allocated_size;

      delete [] __string_array;

      __string_array = new char[__allocated_size];

      strncpy (__string_array, other.__string_array, __string_size);
      
      return *this;
}

void String::__increase () { // OK
      if (__string_size >= __allocated_size) {
            __allocated_size = __string_size*2 + 1;

            char* string_array = new char[__allocated_size];
            
            strncpy (string_array, __string_array, __string_size);

            delete [] __string_array;

            __string_array = string_array;
      }
}

void String::__decrease () { // OK -- this MUST work just fine
      /*if ((__string_size << 2) < __allocated_size) {
            
            __allocated_size /= 2;
            
            char* string_array = new char[__allocated_size];

            strncpy (string_array, __string_array, __string_size);

            delete [] __string_array;

            __string_array = string_array;
      }
      */
}

void String::__resize (size_t new_size) { // This may or may not work
      __allocated_size = new_size;

      char* string_array = new char[new_size];

      strncpy (string_array, __string_array, __string_size);

      delete [] __string_array;

      __string_array = string_array;
}

char& String::operator[] (size_t index) {
      return __string_array[index];
}

const char& String::operator[] (size_t index) const {
      return __string_array[index];
}

size_t String::length() const {
      return __string_size;
}

void String::push_back (char value) {
      __increase ();

      __string_array[__string_size] = value;
      ++__string_size;
}

char String::pop_back () {

      --__string_size;

      char result = __string_array[__string_size];
      __decrease ();

      return result;
}

char& String::front () {
      return __string_array[0];
}

const char& String::front () const {
      return __string_array[0];
}

char& String::back () {
      return __string_array[__string_size - 1];
}

const char& String::back () const {
      return __string_array[__string_size - 1];
}

String& String::operator+= (char value) {
      push_back (value);

      return *this;
}

String& String::operator+= (const String& other) {

      if (__allocated_size < __string_size + other.__string_size) {
            __resize (__string_size + other.__string_size);
      }

      strncpy (__string_array+__string_size, other.__string_array, other.__string_size);
      __string_size += other.__string_size;

      return *this;
}

String& String::operator+= (const char* cstring) {
      size_t other_size = strlen (cstring);

      if (__allocated_size < __string_size + other_size) {
            __resize (__string_size + other_size);
      }

      strncpy (__string_array+__string_size, cstring, other_size);
      __string_size += other_size;

      return *this;
}

const String String::operator+ (char value) const {

      String s (*this);
      s.operator+= (value);

      return s;
}

const String String::operator+ (const String& other) const {
      
      String s (*this);
      s.operator+= (other);
      
      return s;
}

const String operator+ (char value, const String& s) {

      return String (value) + s;
}

const String operator+ (const char* cstring, const String& s) {

      return String (cstring) + s;
}

std::ostream& operator<< (std::ostream& stream, const String& s) {
      for (size_t i = 0; i < s.__string_size; ++i) {
            stream << s[i];
      }

      return stream;
}

std::istream& operator>> (std::istream& stream, String& s) {
      s = String ();
      char c;
      bool br = true;
      
      while (br) {
            if (!stream.get(c)) { return stream; }

            switch (c) {
                  case ' ':
                  case '\t':
                  case '\n':
                        break;

                  default:
                        br = false;
            }
      }

      s.push_back(c);
      
      while (!br) {
            if (!stream.get(c)) { break; }

            switch (c) {
                  case ' ':
                  case '\t':
                  case '\n':
                        br = true;
                        break;
                  default:
                        s.push_back(c);                  
            }
      }

      return stream;
}


bool String::operator== (const String& other) const {
      
      if (other.__string_size != __string_size) {
            return false;
      }

      return strncmp (__string_array, other.__string_array, __string_size) == 0;
            return false;
}

bool String::operator== (const char* cstring) const {
      
      if (strlen (cstring) != __string_size) {
            return false;
      }

      return strncmp (__string_array, cstring, __string_size) == 0;
}

bool String::operator== (char c) const {

      if (__string_size != 1) {
            return false;
      }

      return (__string_array[0] == c);
}

bool operator== (const char *cstring, const String& s) {
      
      return s == cstring;
}

bool operator== (char c, const String& s) {
      
      return s == c;
}

String String::substr (size_t start, size_t count) const{
      String s (count);

      strncpy (s.__string_array, __string_array + start, count);
      s.__string_size = count;

      return s;
}

size_t String::find (const String& s) const {

      if (s.__string_size > __string_size) {
            return __string_size;
      }

      for (size_t i = 0; i <= __string_size-s.__string_size; i++) {
            if (!strncmp (__string_array+i, s.__string_array, s.__string_size)) {
                  return i;
            }
      }

      return __string_size;
}

size_t String::rfind (const String& s) const {
      
      if (s.__string_size > __string_size) {
            return __string_size;
      }

      for (size_t i = __string_size-s.__string_size + 1; i > 0; --i) {
            if (!strncmp (__string_array+i-1, s.__string_array, s.__string_size)) {
                  return i-1;
            }
      }

      return __string_size;
}

bool String::empty () const {
      
      return __string_size == 0;
}

void String::clear () {
      
      delete [] __string_array;
      __string_size = 0;
      __allocated_size = 1;

      __string_array = new char[__allocated_size];
}

#endif

/* OK, let me be honest with you. This contest is a piece of sh*t;*/
