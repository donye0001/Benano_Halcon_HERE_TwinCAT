#pragma once

namespace ads
{
    class Linear
    {
    public:
        struct Request
        {
            bool   new_data;
            double final_position[6];
            double coefficient[6][6];
            double full_time;
            bool   vacuum;
        } req;

        struct Response
        {
            double real_time_ang[6];
            bool   arrive;
        } resp;
    }; //class linear
} // namespace ads