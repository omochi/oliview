#pragma once

#include "./dependency.h"
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
        private:
            int line_index_;
            int byte_offset_;
        };
        
        std::string string() const;
        void set_string(const std::string & value);
        
        std::vector<Ptr<std::string>> line_ptrs() const;
        void set_line_ptrs(const std::vector<Ptr<std::string>> & value);
        
        std::string GetLineAt(int index) const;
        void SetLineAt(int index, const std::string & value);
        
        // utf-8
        std::string GetCharAt(const Position & position) const;
        void SetCharAt(const Position & position, const std::string & chr);
        
        Position begin_position() const;
        Position end_position() const;
        
        Position AdvancePosition(const Position & pos) const;
    private:
        struct StringAccess {
            std::string * string;
            int offset;
            int length;
            Utf8ByteKind kind;
            
            StringAccess(std::string * string,
                         int offset,
                         int length,
                         Utf8ByteKind kind);
        };
        
        StringAccess AccessCharAt(const Position & position) const;
        Position AdvancePositionByte(const Position & pos) const;

        std::vector<Ptr<std::string>> lines_;
    };
}
