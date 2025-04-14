#pragma once

namespace ads
{
    class Linear
    {
    public:
        struct Request
        {
            bool   new_data;
            double fulltime;
            //double final_position[6];
        } req;

        struct Response
        {
            //double real_time_ang[6];
            bool   arrive;
        } resp;
    }; //class linear
} // namespace ads