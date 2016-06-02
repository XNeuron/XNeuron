#ifndef XNEURON_GLOBAL_H
#define XNEURON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XNEURON_LIBRARY)
#  define XNEURONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XNEURONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XNEURON_GLOBAL_H
