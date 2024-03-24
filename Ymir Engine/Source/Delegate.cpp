#include "Delegate.h"

#include "Log.h"

#include "External/mmgr/mmgr.h"

void Delegate::SetCallback(FunctionPtr function) {

    callback = function;

}

void Delegate::Invoke() {

    if (callback != nullptr) {

        callback(); // Invoke the function

    }
    else {

        LOG("[WARNING] Delegate: Callback function not set!");

    }

}