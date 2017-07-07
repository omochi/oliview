#pragma once

#include "./dependency.h"
#include "./string_slice.h"
#include "./utf8.h"

namespace oliview {
    class Text {
    public:
        class Position {
        public:
            Position();
            Position(int line_index, int byte_offset);
            
            RHETORIC_ACCESSOR_TRIVIAL(int, line_index)
            RHETORIC_ACCESSOR_TRIVIAL(int, byte_offset)
            
            bool operator==(const Position & other) const;
            RHETORIC_EQUATABLE_DEFAULT(Position)
            
            bool operator<(const Position & other) const;
            RHETORIC_COMPARABLE_DEFAULT(Position)
            
            std::string ToString() const;
        private:
            int line_index_;
            int byte_offset_;
        };
        
        std::string string() const;
        void set_string(const std::string & value);
        
        std::vector<Ptr<std::string>> lines() const;
        void set_lines(const std::vector<Ptr<std::string>> & value);
        
        int line_num() const;
        
        Ptr<std::string> GetLineAt(int index) const;
        void SetLineAt(int index, const Ptr<std::string> & value);
        
        // utf-8
        StringSlice GetCharAt(const Position & position) const;
        void SetCharAt(const Position & position, const std::string & chr);
        
        Position begin_position() const;
        Position end_position() const;
        
        Position AdvancePosition(const Position & pos) const;
    private:
        struct StringAccess {
            Ptr<std::string> string;
            int offset;
            int length;
            Utf8ByteKind kind;
            
            StringAccess(const Ptr<std::string> & string,
                         int offset,
                         int length,
                         Utf8ByteKind kind);
        };
        
        StringAccess AccessCharAt(const Position & position) const;
        Position SkipBodyBytes(const Position & pos) const;
        Position AdvancePositionByte(const Position & pos) const;

        std::vector<Ptr<std::string>> lines_;
    };
}
