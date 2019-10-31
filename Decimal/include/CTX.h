//#pragma once

#ifndef CTX_VECTOR_H
#define CTX_VECTOR_H

#include <map>
#include <vector>

template <typename Decimal> class CTX_VECTOR
{
private:
    class Instance
    {
    public:
        std::vector<Decimal> P;
        std::vector<Decimal> N;
    };
    static Instance& Init()
    {
        int vector_init_cap = 100;
        CTX_VECTOR::Instance* V = new CTX_VECTOR::Instance();
        V->P.reserve(vector_init_cap);
        V->N.reserve(vector_init_cap);
        Decimal one = Decimal(1);

        Decimal ten = Decimal(10);
        //        DEBUG_COUT(ten.ToDouble());
        V->P.push_back(one);
        V->P.push_back(ten);
        V->N.push_back(one);
        V->N.push_back(one / 10);
        for(int i = 1; i < vector_init_cap; ++i) {
            V->P.push_back(V->P[i] * 10);
            V->N.push_back(V->N[i] / 10);
            //            DEBUG_COUT(i);
            //            DEBUG_COUT(V->N[i].ToDouble());
        }
        //    std::cout << "LINE" << __LINE__ << ":" << V->P[1].ToDouble() << std::endl;
        return *V;
    }

public:
    static Decimal Power10(int index)
    {
        static auto V = Init();
        if(index == 0) {
            return V.P[0];
        }
        if(index > 0) {
            if(index >= (int)V.P.size()) {
                for(int i = V.P.size() - 1; i < index; ++i) {
                    //                    V.P.push_back(V.P[1] * V.P[i]);
                    V.P.push_back(V.P[i] * 10);
                }
            }
            return V.P[index];
        } else {
            index = -index;
            if(index >= (int)V.N.size()) {
                for(int i = V.N.size() - 1; i < index; ++i) {
                    V.N.push_back(V.N[i] / 10);
                }
            }
            return V.N[index];
        }
        DEBUG_COUT(1);
    }
};

template <typename Decimal> class CTX_MAP
{
private:
    typedef std::map<int, Decimal> Instance;
    static Instance& Init()
    {
        Instance* pM = new Instance();
        Instance& M = (*pM);
        M[0] = Decimal(1);
        M[1] = Decimal(10);
        M[-1] = M[1].ToReciprocal();

        int map_init_cap = Decimal::MAX_String_LEN;
        for(int i = 1; i < map_init_cap; ++i) {
            M[i + 1] = M[i] * M[1];
            M[-i - 1] = M[-i] * M[-1];
        }
        return *pM;
    }

public:
    static Decimal Power10(int index)
    {
        static Instance M = Init();
        auto it = M.find(index);
        if(it == M.end()) {
            M[index] = Decimal::power(10, index);
        }
        return M[index];
    }
};

#endif // CTX_VECTOR_H
