(define s1-interpret (lambda (e)
        (if (number? e)
             e
             (error "s1-interpret: cannot evaluate: " e))))
