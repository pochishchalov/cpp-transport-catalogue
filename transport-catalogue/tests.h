#pragma once

#include <cassert>
#include <sstream>

#include "input_reader.h"
#include "stat_reader.h"

namespace transport_catalogue {

    namespace tests {

        using namespace std::literals;

        const double EPSILON = 1e-6;

        inline void Test() {

            TransportCatalogue catalogue;

            //------------------ input test ------------------

            std::istringstream input_create{
                "13\n"
                "Stop Tolstopaltsevo: 55.611087, 37.20829\n"
                "Stop Marushkino: 55.595884, 37.209755\n"
                "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
                "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
                "Stop Rasskazovka: 55.632761, 37.333324\n"
                "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n"
                "Stop Biryusinka: 55.581065, 37.64839\n"
                "Stop Universam: 55.587655, 37.645687\n"
                "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
                "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
                "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
                "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
                "Stop Prazhskaya: 55.611678, 37.603831\n"
            };

            reader::InputReader reader;
            reader.LoadCommands(input_create, catalogue);

            assert((std::abs(catalogue.GetStop("Marushkino"s)->coordinates.lat - 55.595884) < EPSILON &&
                std::abs(catalogue.GetStop("Marushkino"s)->coordinates.lng - 37.209755) < EPSILON));

            assert(catalogue.GetStop("Durdino"s) == nullptr);

            assert(catalogue.GetBus("256")->bus_stops.size() == 6);
            assert(catalogue.GetBus("256")->bus_stops[3]->name == "Biryulyovo Tovarnaya");

            assert(catalogue.GetBus("750")->bus_stops.size() == 5);
            assert(catalogue.GetBus("750")->bus_stops[1]->name == "Marushkino");

            assert(catalogue.GetBus("12"s) == nullptr);

            //------------------ output test ------------------

            std::istringstream input_bus_request{
                "3\n"
                "Bus 256\n"
                "Bus 750\n"
                "Bus 751\n"
            };

            std::ostringstream result_bus_request, result_stop_request;

            std::string right_bus_result{
                "Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
                "Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
                "Bus 751: not found\n"
            };

            std::istringstream input_stop_request{
                "3\n"
                "Stop Samara\n"
                "Stop Prazhskaya\n"
                "Stop Biryulyovo Zapadnoye\n"
            };

            std::string right_stop_result{
                "Stop Samara: not found\n"
                "Stop Prazhskaya: no buses\n"
                "Stop Biryulyovo Zapadnoye: buses 256 828\n"
            };

            reader::GetStat(catalogue, input_bus_request, result_bus_request);
            reader::GetStat(catalogue, input_stop_request, result_stop_request);

            assert((result_bus_request.str() == right_bus_result));
            assert((result_stop_request.str() == right_stop_result));
        }
    }
}