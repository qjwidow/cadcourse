#include <iostream>
#include <assert.h>
#include "HalfEdgeDS.hpp"

Solid::Solid() : Sprev(nullptr), Snext(nullptr), Sfaces(nullptr), Sedges(nullptr),
    faceNum(0), edgeNum(0){

}

void Solid::add_Face(Face *newFace){
    assert(newFace != nullptr);

    if(Sfaces == nullptr){
        Sfaces = newFace;
        newFace->Fprev = nullptr;
        newFace->Fnext = nullptr;
    }
    else{
        Face *ptr = Sfaces;
        while(ptr->Fnext != nullptr) ptr = ptr->Fnext;
        ptr->Fnext = newFace;
        newFace->Fprev = ptr;
        newFace->Fnext = nullptr;
    }

    newFace->Fsolid = this;
    ++faceNum;
}

void Solid::del_Face(Face *face){
    assert(face != nullptr);

    if(this->Sfaces == face){
        this->Sfaces = face->Fnext;
    }
    else{
        face->Fprev->Fnext = face->Fnext;
        if(face->Fnext != nullptr){
            face->Fnext->Fprev = face->Fprev;
        }
    }
    delete face;
    --faceNum;
}

void Solid::add_Edge(Edge *newEdge){
    assert(newEdge != nullptr);

    if(Sedges == nullptr){
        Sedges = newEdge;
        newEdge->Eprev = nullptr;
        newEdge->Enext = nullptr;
    }
    else{
        Edge *ptr = Sedges;
        while(ptr->Enext != nullptr) ptr = ptr->Enext;
        ptr->Enext = newEdge;
        newEdge->Eprev = ptr;
        newEdge->Enext = nullptr;
    }
    ++edgeNum;
}

void Solid::del_Edge(Edge *e){
    assert(e != nullptr);

    if(this->Sedges == e){
        this->Sedges = e->Enext;
    }
    else{
        e->Eprev->Enext = e->Enext;
        if(e->Enext != nullptr){
            e->Enext->Eprev = e->Eprev;
        }
    }
    delete e;
    --edgeNum;
}

Face::Face() : Fprev(nullptr), Fnext(nullptr), Fsolid(nullptr), Floops(nullptr), LoopNum(0),
    color(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f){
}

void Face::add_Loop(Loop *newLoop){
    assert(newLoop != nullptr);

    if(Floops == nullptr){
        Floops = newLoop;
        newLoop->Lprev = nullptr;
        newLoop->Lnext = nullptr;
    }
    else{
        Loop *ptr = Floops;
        while(ptr->Lnext != nullptr) ptr = ptr->Lnext;
        ptr->Lnext = newLoop;
        newLoop->Lprev = ptr;
        newLoop->Lnext = nullptr;
    }

    newLoop->Lface = this;
    ++LoopNum;
}

Loop::Loop() : Lprev(nullptr), Lnext(nullptr), Lface(nullptr), Lhe(nullptr), inner(false){

}

void Loop::add_HE(HalfEdge *he1, HalfEdge *he2){
    assert(he1 != nullptr);
    assert(he2 != nullptr);

    he1->HEnext = he2;
    he2->HEprev = he1;

    if(Lhe == nullptr){
        Lhe = he1;
        he1->HEprev = he2;
        he2->HEnext = he1;
    }
    else{
        Vertex *v1 = he1->v;
        Vertex *v2 = he2->v;
        HalfEdge *ptr = Lhe;
        do{
            if(ptr->HEnext->v == v1) break;
            ptr = ptr->HEnext;
        }while(ptr != Lhe);
        if(ptr->HEnext->v != v1){
            std::cout << "error: v1 doesn't exist in current loop" << std::endl;
            exit(-1);
        }

        he1->HEprev = ptr;
        he2->HEnext = ptr->HEnext;
        ptr->HEnext->HEprev = he2;
        ptr->HEnext = he1;
    }

    he1->HEloop = this;
    he2->HEloop = this;
}

HalfEdge * Loop::findHE(Vertex *v){
    assert(v != nullptr);

    HalfEdge *ptr = Lhe;
    if(ptr == nullptr){
        return nullptr;
    }
    else{
        do{
            if(ptr->v == v) break;
            ptr = ptr->HEnext;
        }while(ptr != Lhe);
        if(ptr->v == v){
            return ptr;
        }
        else{
            return nullptr;
        }
    }
}

void Loop::debug(){
    // print link table of halfedge for debug
    HalfEdge *ptr = Lhe;
    do{
        std::cout << "(" << ptr->v->cor.x << "," << ptr->v->cor.y << "," <<
            ptr->v->cor.z << ") --> ";
        ptr = ptr->HEnext;
    }while(ptr != Lhe);
    std::cout << std::endl;
}

Edge::Edge() : Eprev(nullptr), Enext(nullptr), he1(nullptr), he2(nullptr){

}

void Edge::connect(HalfEdge *he1, HalfEdge *he2){
    he1->couple = he2;
    he2->couple = he1;
    he1->HEedge = this;
    he2->HEedge = this;
    this->he1 = he1;
    this->he2 = he2;
}

HalfEdge::HalfEdge() : HEprev(nullptr), HEnext(nullptr), HEloop(nullptr), couple(nullptr),
    HEedge(nullptr), v(nullptr){

}

Vertex::Vertex(){

}