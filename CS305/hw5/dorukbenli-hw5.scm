(define get-operator (lambda (op-symbol)
  (cond
    ((eq? op-symbol '+) +)
    ((eq? op-symbol '*) *)
    ((eq? op-symbol '-) -)
    ((eq? op-symbol '/) /)
    (else ((display "cs305: ERROR\n\n")(repl env))))))

(define built-in-operator? (lambda (op-symbol) ;checks if given ops are built in
  (or (equal? op-symbol '+)(equal? op-symbol '-)(equal? op-symbol '*)(equal? op-symbol '/))
))


(define check-env? (lambda (e env)
  (if (null? env)
      #f
      (if (eq? (caar env) e)
          #t
          (check-env? e (cdr env))))))

(define var-binding-list? (lambda (e)
  (cond
    ((null? e) #t) 
    ((and (list? (car e)) (null? (car e))) #f) ;(()) does not trigger, check later
    ((and (list? (car e)) (= (length (car e)) 2) (symbol? (caar e)) (expr? (cadar e)))
     (var-binding-list? (cdr e)))
    (else #f))))


(define formal-binding-list? (lambda (e)
	(and (list? e) (symbol? (car e)) (or (null? (cdr e)) (formal-binding-list? (cdr e))))
))

(define get-value (lambda (var old-env new-env)
    (cond
        ((null? new-env) (display "cs305: ERROR \n\n") (repl old-env))
        ((equal? (caar new-env) var) (cdar new-env))
        (else (get-value var old-env (cdr new-env))))))

(define is-procedure? (lambda (e env)
  (and (symbol? e)
       (check-env? e env)
       (let ((val (get-value e env env)))
         (lambda-expr? val)))))

(define extend-env (lambda (var val old-env)
        (cons (cons var val) old-env)))

(define define-expr? (lambda (e)
         (and (list? e) (= (length e) 3) (eq? (car e) 'define) (symbol?(cadr e)))))

(define if-expr? (lambda (e)
    ;if this is true, then we have an if statement 
    (and (list? e) (eq? (length e) 4) (eq? (car e) 'if))
))
(define let-expr? (lambda (e)
    (and (list? e)
         (eq? (car e) 'let)
         (=(length e) 3)
         (or (eq? (cadr e) '())
             (and (list? (cadr e))
                  (not (and (null? (car (cadr e))) (null? (cdr (cadr e)))))
                  (var-binding-list? (cadr e)))))))

(define lambda-expr? (lambda (e)
    ;if this evaluates to true, then we have a lambda-expr
    (and (list? e) (and (eq? (car e) 'lambda)(formal-binding-list? (cadr e)) (expr? (caddr e)) (not (define-expr? (caddr e)))))
))

(define let-func (lambda (e env)
  (let ((bindings (cadr e))
        (body (caddr e)))
    (if (null? bindings)
        (s7 body env)
        (let* ((vars (map car bindings))
               (vals (map (lambda (val) (s7 val env)) (map cadr bindings)))
               (new-env (extend-env-list vars vals env)))
          (s7 body new-env))))))

(define extend-env-list (lambda (vars vals env)
  (if (null? vars)
      env
      (extend-env-list (cdr vars) (cdr vals) (cons (cons (car vars) (car vals)) env)))))

(define (lambda-func e env)
  (if (= (length (cadar e)) (length (cdr e)))
      (let* (
            (args (map (lambda (arg) (s7 arg env)) (cdr e)))
            (param-env (map cons (cadar e) args))
            (new-env (append param-env env)))
        (s7 (caddar e) new-env))
      (begin
        (display "cs305: ERROR \n\n")
        (repl env))))


(define (evaluate-if-expr e env)
  (if (if-expr? e)
      (if (>= (length e) 4)
          (let ((val (if (not (= (s7 (cadr e) env) 0))
                         (s7 (caddr e) env)
                         (s7 (cadddr e) env))))
            val)
          (begin (display "cs305: ERROR\n\n") (repl env)))
      (begin (display "cs305: ERROR\n\n") (repl env))))



(define in-line-expr? (lambda (e)
  (and (list? e)(or (eq? '+ (car e)) (eq? '* (car e)) (eq? '- (car e)) (eq? '/ (car e)))(> (length e) 2)) 
))

(define (calculate-value e env)
  (let* (
         (operands (map (lambda (operand) (s7 operand env)) (cdr e)))
         (operator (get-operator (car e))))
    (if (not (eq? operator (lambda () ((display "cs305: ERROR\n\n")(repl env)))))
        (apply operator operands)
        ((display "cs305: ERROR\n\n")(repl env)))))

(define expr? (lambda (e)
  (or 
    (number? e)
    (symbol? e)
    (if-expr? e) ; <if>
    (let-expr? e) ; <let>
    (lambda-expr? e) ; <lambda>
    (and (list? e) ; ( <operator> <operand_list> )
         (or (built-in-operator? (car e)) (expr? (car e)))
         (operand-list? (cdr e))) ;
  )
))

(define operand-list? (lambda (lst)
  (or (null? lst)
      (and (expr? (car lst)) (operand-list? (cdr lst)))
  )
))


(define s7 (lambda (e env)
  (cond
    ((not (expr? e)) (begin (display "cs305: ERROR\n\n") (repl env)))
    ((number? e) e)
    ((or (built-in-operator? e) (is-procedure? e env)) (begin (display "cs305: [PROCEDURE]\n\n") (repl env)))
    ((and (symbol? e) (check-env? e env)) (get-value e env env))
    ((not (list? e)) (display "cs305: ERROR \n\n") (repl env))
    ((if-expr? e) (evaluate-if-expr e env))
    ((let-expr? e) (let-func e env))
    ((lambda-expr? e) e)
    ((lambda-expr? (car e)) (lambda-func e env))
    ((in-line-expr? e) (calculate-value e env))
    (else (s7 (append (list (get-value (car e) env env)) (cdr e)) env))
  )))

(define repl (lambda (env)
   (let* (
           (dummy1 (display "cs305> "))
           (expr (read))
           (new-env (if (define-expr? expr) 
                        (extend-env (cadr expr) (s7 (caddr expr) env) env)
                        env
                    ))
           (val (if (define-expr? expr)
                    (cadr expr)
                    (s7 expr env)
                ))
           (dummy2 (display "cs305: "))
           (dummy3 (display val))
           (dummy4 (newline))
           (dummy5 (newline))
          )
          (repl new-env))))

(define cs305
  (lambda ()
    (repl '())
  )
)
