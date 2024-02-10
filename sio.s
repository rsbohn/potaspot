    ;; Call SIO
    .export _siov
    
_siov:
    JSR $E459
    LDA $0303
    RTS