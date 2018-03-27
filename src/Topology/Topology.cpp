#ifdef OS_WIN32
#include <Windows.h>
#elif (defined(OS_LINUX))
#include <dlfcn.h>
#endif

#include "hurricane/topology/ITopology.h"

using hurricane::topology::ITopology;

namespace hurricane {
    namespace base {
#ifdef OS_WIN32
        ITopology* LoadTopology(const std::string& fileName) {
            HINSTANCE hInstance = LoadLibrary(fileName.c_str());

            typedef ITopology* (TopologyGetter)();

            TopologyGetter GetTopology = (TopologyGetter)GetProcAddress(hInstance, "GetTopology");

            return topology;
        }
#elif (defined(OS_LINUX))
        ITopology* LoadTopology(const std::string& fileName) {
            void *libm_handle = NULL;
            typedef ITopology* (TopologyGetter)();
            char *errorInfo;

            libm_handle = dlopen(fileName.c_str(), RTLD_LAZY);

            if( !libm_handle ) {
                printf("Open Error:%s.\n", dlerror);
                return 0;
            }

            TopologyGetter GetTopology = dlsym(libm_handle, "GetTopology");
            errorInfo = dlerror();

            if( errorInfo != NULL ) {
                printf("Dlsym Error:%s.\n", errorInfo);
                return 0;
            }

            ITopology* topology = GetTopology();

            dlcolse(libm_handle);

            return topology;
        }
#endif
    }
}