#ifndef BATCHSTATUS_H
#define BATCHSTATUS_H

enum class BatchStatus {
    AVAILABLE,
    NEAR_EXPIRATION,
    EXPIRED,
    DEPLETED
};

#endif