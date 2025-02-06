cmake_minimum_required(VERSION 3.30)
project(Supermarket C)

set(CMAKE_C_STANDARD 11)

add_executable(Supermarket main.c main.h Date.c Date.h FileHelper.c FileHelper.h General.c General.h GeneralList.c GeneralList.h Product.c Product.h ShoppingCart.c ShoppingCart.h ShoppingItem.c ShoppingItem.h SuperFile.c SuperFile.h Supermarket.c Supermarket.h vtable.h Customer.c Customer.h ClubMember.c ClubMember.h
        myMacros.h)
