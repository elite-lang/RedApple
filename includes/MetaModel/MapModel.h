#ifndef MAP_MODEL_H
#define MAP_MODEL_H

#include "MetaModel/MetaModel.h"
#include <map>
#include <string>
#include "idmap.h"

class MapModel : public MetaModel {
public:
    MapModel (const string& name);
    virtual ~MapModel ();

    id* find(const string& name);

protected:
    /* data */
    std::map< std::string, id* > data;

};




#endif /* end of include guard: MAP_MODEL_H */
