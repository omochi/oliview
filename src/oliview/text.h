#pragma once

#include "./dependency.h"
#include "./math.h"
#include "./string_slice.h"
#include "./text_line_reader.h"
#include "./utf8.h"

namespace oliview {
    /*
     改行がある行には必ず次の行があることとする。
     
     "" => [""]
     "a" => ["a"]
     "a\n" => ["a\n", ""]
     "a\nb" => ["a\n", "b"]
     
     バイナリフリーで不正なバイトは1バイトずつ読まれる
     */
    class Text {
    public:
        class Index {
        public:
            Index();
            Index(size_t line, size_t byte);
            
            RHETORIC_ACCESSOR_TRIVIAL(size_t, line)
            RHETORIC_ACCESSOR_TRIVIAL(size_t, byte)
            
            bool operator==(const Index & other) const;
            RHETORIC_EQUATABLE_DEFAULT(Index)
            
            bool operator<(const Index & other) const;
            RHETORIC_COMPARABLE_DEFAULT(Index)
            
            std::string ToString() const;
        private:
            size_t line_;
            size_t byte_;
        };
        
        Text();
        Text(const std::string & value);
        
        std::string string() const;
        void set_string(const std::string & value);
        
        std::vector<Ptr<std::string>> lines() const;
        void set_lines(const std::vector<Ptr<std::string>> & value);
        
        size_t line_num() const;
        
        Ptr<std::string> GetLineAt(size_t line_index) const;
        void SetLineAt(size_t line_index, const Ptr<std::string> & value);
        
        // utf-8
        StringSlice GetCharAt(const Index & index) const;
        void SetCharAt(const Index & index, const std::string & chr);
        
        Index begin_index() const;
        Index end_index() const;
        
        Index AdvanceIndex(const Index & pos) const;
        
        bool CheckIndex(const Index & pos) const;
        
        void Insert(const Index & index,
                    const Ptr<Text> & text,
                    Index * end);
        void Delete(const Index & begin,
                    const Index & end);
    private:
        struct StringAccess {
            Ptr<std::string> string;
            size_t offset;
            size_t length;
            Optional<Utf8ByteKind> kind;
            
            StringAccess(const Ptr<std::string> & string,
                         size_t offset,
                         size_t length,
                         Optional<Utf8ByteKind> kind);
        };
        
        void FixLastLine();
        
        StringAccess AccessCharAt(const Index & position) const;
        
        std::vector<Ptr<std::string>> lines_;
    };
}
