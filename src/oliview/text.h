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
     
     Indexは行については 0 ..< line_num
     バイトについては 0 ..<= line.size()
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
        Text(const Text & other);
        Text & operator=(const Text & other);
        
        std::string string() const;
        void set_string(const std::string & value);
        
        std::vector<Ptr<const std::string>> lines() const;
        void set_lines(const std::vector<Ptr<const std::string>> & value);
        
        size_t line_num() const;
        
        Ptr<const std::string> GetLineAt(size_t line_index) const;
        void SetLineAt(size_t line_index, const Ptr<const std::string> & value);
        
        // utf-8
        std::string GetCharAt(const Index & index) const;
        void SetCharAt(const Index & index, const std::string & chr);
        
        Index begin_index() const;
        Index end_index() const;
        
        Index AdvanceIndex(const Index & index) const;
        Index AdvanceIndexNoSkip(const Index & index) const;
        Index BackIndex(const Index & index) const;
        Index BackIndexNoSkip(const Index & index) const;
        
        bool CheckIndex(const Index & index) const;
        
        void InsertAt(const Index & index,
                      const Ptr<const Text> & text,
                      Index * end_index);
        void DeleteAt(const Index & begin,
                      const Index & end);
        
        Index GetNewlineIndex(size_t line_index) const;
    private:
        template <typename STR>
        struct StringAccess {
            StringSliceBase<STR> string;
            Option<Utf8ByteKind> kind;
            
            StringAccess(const StringSliceBase<STR> & string,
                         Option<Utf8ByteKind> kind):
            string(string),
            kind(kind)
            {}
        };
        
        /*
         文字単位で進む。
         行末で1度停止して、次に行頭に移動する。
         */
        Index _AdvanceIndex(const Index & index) const;
        
        void FixLastLine();
        
        StringAccess<const std::string> AccessCharAt(const Index & index) const;
        StringAccess<std::string> AccessCharAt(const Index & index);
        Index MayLineWrapIndex(const Index & index) const;
        
        std::vector<Ptr<std::string>> lines_;
    };
}
