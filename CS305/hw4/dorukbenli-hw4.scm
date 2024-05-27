(define (twoOperatorCalculator lst)
  (define (eval-expr lst acc)
    (cond ((null? lst) acc)
          ((number? (car lst)) (eval-expr (cdr lst) (car lst)))
          ((eq? (car lst) '+) (eval-expr (cddr lst) (+ acc (cadr lst))))
          ((eq? (car lst) '-) (eval-expr (cddr lst) (- acc (cadr lst))))))

  (eval-expr lst 0))

(define (fouroperatorcalculator lst)
  (if (null? (cdr lst))
      lst
      (let ((op (cadr lst)))
        (cond ((eqv? '* op)
               (fouroperatorcalculator (cons (* (car lst) (caddr lst)) (cdddr lst))))
              ((eqv? '/ op)
               (fouroperatorcalculator (cons (/ (car lst) (caddr lst)) (cdddr lst))))
              (else
               (cons (car lst) (fouroperatorcalculator (cdr lst))))))))

(define (calculatorNested lst)
  (cond
    ((null? lst) lst)
    ((and (list? (car lst)) (null? (cdr lst)))
     (list (twoOperatorCalculator (fourOperatorCalculator (calculatorNested (car lst))))))
    ((list? (car lst))
     (append (list (twoOperatorCalculator (fourOperatorCalculator (calculatorNested (car lst)))))
             (calculatorNested (cdr lst))))
    ((null? (cdr lst)) lst)
    (else (append (list (car lst)) (calculatorNested (cdr lst))))))


(define (checkOperators lst)
  (cond
    ((not(list? lst))#f)
    ((null? lst) #f) 
    ((and (number? (car lst)) (null? (cdr lst))) #t)
    ((pair? (car lst)) (checkOperators (car lst)))
    ((and (number? (car lst))
          (or (eq? '- (cadr lst))
              (eq? '+ (cadr lst))
              (eq? '/ (cadr lst))
              (eq? '* (cadr lst))))
     (checkOperators (cddr lst)))
    ((pair? (car lst))
     (and (checkOperators (car lst)) (checkOperators (cddr lst))))
    (else #f)))
    
(define (calculator lst)
  (cond
    ((checkOperators lst) (twoOperatorCalculator (fourOperatorCalculator (calculatorNested lst))))
    (else #f)))
