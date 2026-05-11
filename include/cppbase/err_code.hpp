#pragma once

#ifndef CPPBASE_ERR_CODE_HPP
#define CPPBASE_ERR_CODE_HPP

namespace cppbase {

/**
 * @brief Error code definitions for cppbase library
 * @details Comprehensive error codes covering general, status, operation,
 *          limit, and validation categories
 */
enum ErrorCode {
  /* General */
  EC_OK = 0,    /* Success */
  EC_ERROR = 1, /* General error */

  /* Status */
  EC_BUSY = 100,        /* Busy */
  EC_AGAIN = 101,       /* Retry */
  EC_TIMEOUT = 102,     /* Timeout */
  EC_EXISTS = 103,      /* Already exists */
  EC_NOT_EXIST = 104,   /* Does not exist */
  EC_CHANGED = 105,     /* Changed */
  EC_RESET = 106,       /* Reset */
  EC_NOT_READY = 107,   /* Not ready */
  EC_DELETED = 108,     /* Deleted */
  EC_PAUSED = 109,      /* Paused */
  EC_RUNNING = 110,     /* Running */
  EC_IN_PROCESS = 111,  /* In progress */
  EC_STOPPED = 112,     /* Stopped */
  EC_NOT_INIT = 113,    /* Not initialized */
  EC_NOT_ENABLE = 114,  /* Not enabled */
  EC_DISABLED = 115,    /* Disabled */
  EC_FINISHED = 116,    /* Finished */
  EC_NOT_CONNECT = 117, /* Not connected */
  EC_PEER_ERROR = 118,  /* Peer error */

  /* Operation / Permission */
  EC_END = 200,               /* End of */
  EC_NOT_SUPPORT = 201,       /* Not supported */
  EC_NOT_IMPLEMENT = 202,     /* Not implemented */
  EC_OPEN_FAILED = 203,       /* Open failed */
  EC_CLOSE_FAILED = 204,      /* Close failed */
  EC_ADD_FAILED = 205,        /* Add failed */
  EC_DELETE_FAILED = 206,     /* Delete failed */
  EC_MODIFY_FAILED = 207,     /* Modify failed */
  EC_QUERY_FAILED = 208,      /* Query failed */
  EC_READ_FAILED = 209,       /* Read failed */
  EC_WRITE_FAILED = 210,      /* Write failed */
  EC_NO_PERMISSION = 211,     /* No permission */
  EC_DENY_OPERATION = 212,    /* Operation denied */
  EC_ILLEGAL_OPERATION = 213, /* Illegal operation */
  EC_REPEAT_OPERATION = 214,  /* Repeat operation */
  EC_SERVICE_NOT_FOUND = 215, /* Service not found */

  /* Limit */
  EC_FULL = 300,             /* Full */
  EC_NO_SPACE = 301,         /* No space */
  EC_NOT_FOUND = 302,        /* Not found */
  EC_OUT_OF_RANGE = 303,     /* Out of range */
  EC_OUT_OF_MEMORY = 304,    /* Out of memory */
  EC_OUT_OF_RESOURCES = 305, /* Out of resources */
  EC_BUFFER_OVERFLOW = 306,  /* Buffer overflow */
  EC_BUFFER_OVERLAP = 307,   /* Buffer overlap */

  /* Validation */
  EC_LOCKED = 400,           /* Locked */
  EC_IS_NULL = 401,          /* Is null */
  EC_NO_DATA = 402,          /* No data */
  EC_NOT_SAFE = 403,         /* Not safe */
  EC_UNREACHABLE = 404,      /* Unreachable */
  EC_REACHED_LIMIT = 405,    /* Reached limit */
  EC_NOT_UNIQUE = 406,       /* Not unique */
  EC_NOT_ACCESS = 407,       /* Not accessible */
  EC_BAD_ENV = 408,          /* Bad environment */
  EC_BAD_TYPE = 409,         /* Bad type */
  EC_BAD_FORMAT = 410,       /* Bad format */
  EC_BAD_CONFIG = 411,       /* Bad configuration */
  EC_BAD_COMMAND = 412,      /* Bad command */
  EC_BAD_LENGTH = 413,       /* Bad length */
  EC_CHECK_FAILED = 414,     /* Check failed */
  EC_INVALID_PARAM = 415,    /* Invalid parameter */
  EC_INVALID_DATA = 416,     /* Invalid data */
  EC_INVALID_NAME = 417,     /* Invalid name */
  EC_INVALID_PASSWORD = 418, /* Invalid password */
  EC_INVALID_ACCESS = 419,   /* Invalid access */
  EC_INVALID_HANDLE = 420,   /* Invalid handle */
  EC_INVALID_LEVEL = 421,    /* Invalid level */
  EC_PARSE_FAILED = 422,     /* Parse failed */
};

}  // namespace cppbase

#endif  // CPPBASE_ERR_CODE_HPP