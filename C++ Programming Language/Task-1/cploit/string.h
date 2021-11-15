class String {
      public:
            [[nodiscard]] char** operator=(String& other) {return nullptr;}
      private:
            String (String& other) { return; }
            int x;
};
