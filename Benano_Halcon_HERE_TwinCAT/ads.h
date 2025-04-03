#pragma once
namespace ads
{
    namespace index
    {
        struct group
        {
            enum : unsigned long
            {
                MOTION = 0x80000000,
                GET = 0x80000001
            };
        };
        struct motion_offset
        {
            enum : unsigned long
            {
                LINEAR = 0x00000000,   //µo°e
                PTP = 0x00000001
            };
        };
        struct get_offset
        {
            enum : unsigned long
            {
                ANGLE = 0x00000000, //±µ¦¬
            };
        };
    } // namespace index
} // namespace ads
