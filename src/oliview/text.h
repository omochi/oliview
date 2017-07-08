#pragma once

#include "./dependency.h"
#include "./math.h"
#include "./string_slice.h"
#include "./utf8.h"

namespace oliview {
    class Text {
    public:
        class Position {
        public:
            Position();
            Position(size_t line_index, size_t byte_offset);
            
            RHETORIC_ACCESSOR_TRIVIAL(size_t, line_index)
            RHETORIC_ACCESSOR_TRIVIAL(size_t, byte_offset)
            
            bool operator==(const Position & other) const;
            RHETORIC_EQUATABLE_DEFAULT(Position)
            
            bool operator<(const Position & other) const;
            RHETORIC_COMPARABLE_DEFAULT(Position)
            
            std::string ToString() const;
        private:
            size_t line_index_;
            size_t byte_offset_;
        };
        
        std::string string() const;
        void set_string(const std::string & value);
        
        std::vector<Ptr<std::string>> lines() const;
        void set_lines(const std::vector<Ptr<std::string>> & value);
        
        size_t line_num() const;
        
        Ptr<std::string> GetLineAt(size_t index) const;
        void SetLineAt(size_t index, const Ptr<std::string> & value);
        
        // utf-8
        StringSlice GetCharAt(const Position & position) const;
        void SetCharAt(const Position & position, const std::string & chr);
        
        Position begin_position() const;
        Position end_position() const;
        
        Position AdvancePosition(const Position & pos) const;
        Position FixPosition(const Position & pos) const;
        
        void Insert(const Position & position,
                    const std::string & string,
                    Position * result_position);
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
        
        StringAccess AccessCharAt(const Position & position) const;
        Position SkipBodyBytes(const Position & pos) const;
        Position AdvancePositionByte(const Position & pos) const;

        std::vector<Ptr<std::string>> lines_;
    };
}
