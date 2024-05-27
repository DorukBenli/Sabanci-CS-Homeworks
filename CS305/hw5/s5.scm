(define get-operator (lambda (op-symbol)
    (cond
       ( (eq? op-symbol '+) +)
       ( (eq? op-symbol '-) -)
       ( (eq? op-symbol '*) *)
       ( (eq? op-symbol '/) /)
       ( else 	(error "s5-interpret cannot handle the operator: " e)))))


(define s5-interpret (lambda (e)
   (cond 
     ( (number? e) e )
     ( (not (list? e))			(error "s5-interpret cannot evaluate: " e))
     ( else (let 
                ( (operator (get-operator (car e)))
                  (operands (map s5-interpret (cdr e))) )
                  (apply operator operands))))))
                                           
                                           


