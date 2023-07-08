#ifndef NPBioLayerData_hh
#define NPBioLayerData_hh

#ifdef WIN32
	#ifdef NPLIBRARY_BUILD_DLL
		#define NPLIBRARY_API __declspec(dllexport)
	#else
		#define NPLIBRARY_API __declspec(dllimport)
	#endif
#else
	#define NPLIBRARY_API
#endif

#include "G4Types.hh"
#include "G4String.hh"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <vector>
#include <math.h>
#ifdef _WIN32
#include <boost/config/compiler/visualc.hpp>
#else
// @todo: check another variants
#include <boost/config/compiler/gcc.hpp>  
#include <inttypes.h>
#endif

#ifdef NPLibrary_USE_BOOST_SERIALIZATION

#include <boost/serialization/map.hpp> 
#include <boost/serialization/string.hpp> 
#include <boost/serialization/list.hpp> 
#include <boost/serialization/vector.hpp>
#include <boost/serialization/level_enum.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/collection_traits.hpp>
#include <boost/serialization/is_bitwise_serializable.hpp>
#include <boost/functional/hash.hpp>
//#include <boost/archive/basic_archive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/polymorphic_binary_iarchive.hpp>
//#include <boost/archive/polymorphic_binary_oarchive.hpp>

#endif

#ifdef G4MULTITHREADED
#include "G4Threading.hh"
#include "G4AutoLock.hh"
#endif

struct NPLIBRARY_API PairHash {
  template <class T1, class T2>
  std::size_t operator () (const std::pair<T1, T2>& p) const;
}; // eof struct PairHash

namespace NPLibrary {
  namespace NPBioLayerData {
#ifdef _WIN32
    typedef __int64 myIntType;
#else
    typedef int64_t myIntType;
#endif

    class NPLIBRARY_API NPBioScoringLayer {
    public:
      double depEnergy;
      double letBase;
      double letd;
      double mev2gy;
      double dose;
      double lnsurvival;
      myIntType nEvents;

      void Nullify();
      void setMeV2GyConversion(double);
      void Update(double, double);
      void Update(NPBioScoringLayer);
      void Update(NPBioScoringLayer*);
      NPBioScoringLayer();
      NPBioScoringLayer(double, double);
      NPBioScoringLayer(NPBioScoringLayer&);
      void calculateLetD();
      void calculateDose();

      NPBioScoringLayer operator+ (const NPBioScoringLayer&);
      NPBioScoringLayer& operator+= (const NPBioScoringLayer&);

#ifdef NPLibrary_USE_BOOST_SERIALIZATION
    private:
      friend class boost::serialization::access;
      template <class Archive> void serialize(Archive& ar, const unsigned int version) {
        ar& depEnergy& letBase& nEvents& letd & mev2gy & dose & lnsurvival;
      }
#endif
    };  // eof class NPBioScoringLayer



    typedef std::map<myIntType, NPBioScoringLayer*> NPBioScoringMapPointer;
    typedef std::pair<myIntType, NPBioScoringLayer*> NPBioScoringMapPointerPair;
    typedef std::pair<myIntType, myIntType> myIntPair;
    //typedef std::map<myIntPair, NPBioScoringLayer*> NPBioTotalScoringMapPointer;
    typedef std::unordered_map<myIntPair, NPBioScoringLayer*, PairHash> NPBioTotalScoringMapPointer;
    typedef std::pair<myIntPair, NPBioScoringLayer*> NPBioTotalScoringMapPointerPair;

    //typedef std::unordered_map<myIntPair, double, PairHash> NPBioTotalDose;
    //typedef std::unordered_map<myIntPair, double, PairHash> NPBioLetBase;
    typedef std::unordered_map<myIntType, double> NPBioTotalDose;
    typedef std::unordered_map<myIntType, double> NPBioLetBase;
    //typedef std::map<int, NPBioScoringMapPointer*> NPBioTotalScoringMapPointer;
    //typedef std::pair<int, NPBioScoringMapPointer*> NPBioTotalScoringMapPointerPair;



    class NPBioVoxelMain {
    public:
#ifdef G4MULTITHREADED
      NPLIBRARY_API NPBioVoxelMain(bool isMaster = true);
#else
      NPLIBRARY_API NPBioVoxelMain();
#endif
      virtual NPLIBRARY_API ~NPBioVoxelMain();
      static NPLIBRARY_API NPBioVoxelMain* Instance();

#ifdef G4MULTITHREADED
      static NPBioVoxelMain* vxMasterInstance;
      static G4ThreadLocal NPBioVoxelMain* vxInstance;
#else
      static NPBioVoxelMain* vxInstance;
#endif

      void NPLIBRARY_API setVoxelData(int, int, int);

      myIntType NPLIBRARY_API getNumFromXYZCoord(myIntType ix, myIntType iy, myIntType iz);
      myIntType NPLIBRARY_API getXCoordFromNum(myIntType bn);
      myIntType NPLIBRARY_API getYCoordFromNum(myIntType bn);
      myIntType NPLIBRARY_API getZCoordFromNum(myIntType bn);
      inline myIntType NPLIBRARY_API getTopCoord() { return _xLim * _yLim * _zLim; };

      void NPLIBRARY_API addData(myIntType, myIntType, double, double);

      void NPLIBRARY_API finalize();
      void NPLIBRARY_API _merge(NPBioTotalScoringMapPointer);

      int NPLIBRARY_API addMeV2GyData(G4String);
      int NPLIBRARY_API addMeV2GyData(G4double);

      void NPLIBRARY_API dumpDoseDepsInFiles(int, G4String);
      void NPLIBRARY_API dummyPrint();

    private:
      NPBioTotalScoringMapPointer dosemap;
      NPBioTotalDose dosemap1;
      NPBioLetBase letmap1;
      double *dosemap2;
      double* letmap2;
      int _xLim, _yLim, _zLim;
      bool isLimSet;
    };
  };
};

#endif