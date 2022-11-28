#ifndef SJPSLIB_CHANNEL_H

  #define SJPSLIB_CHANNEL_H

  #ifndef __ASSERT_H_
    #include <assert.h>
  #endif

  #ifndef _INC_STDLIB
    #include <stdlib.h>
  #endif

  #include "types.h"

  struct Channel {
    u64 msgcap, msgc;
    string *msgv;
  };

  typedef struct Channel Channel;

  /* Creates a new FIFO channel struct with the specified initial capacity. */
  Channel channel_create(u64 capacity) {
    Channel ch;
    ch.msgcap = capacity;
    ch.msgc = 0;
    ch.msgv = (string*) malloc(sizeof(string*) * ch.msgcap);
    return ch;
  }

  /* Push a message to the end of the list. */
  void channel_push(Channel *ch, string msg) {
    assert (ch != NULL);
    ch->msgv[ch->msgc++] = msg;
    if (ch->msgc == ch->msgcap) {
      ch->msgcap *= 2;
      ch->msgv = (string*) realloc(ch->msgv, ch->msgcap);
    }
  }

  /* Pop a message from the beginning of the list. */
  string channel_pop(Channel *ch) {
    assert (ch != NULL);
    if (ch->msgc > 0) {
      string msg = ch->msgv[0];
      ch->msgc -= 1;
      for (u64 i = 0; i < ch->msgc; i++) {
        ch->msgv[i] = ch->msgv[i + 1];
      }
      return msg;
    }
    return NULL;
  }

  /* Free up all memory used by the channel. */
  void channel_free(Channel *ch) {
    assert (ch != NULL);
    free(ch->msgv);
  }

#endif