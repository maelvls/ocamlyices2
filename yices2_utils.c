#include <yices.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/custom.h>
#include <stdio.h>
#include <caml/fail.h>

#include "yices2.h"

struct custom_operations ocamlyices_context_ops = {
  "ocamlyices.context",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

struct custom_operations ocamlyices_ctx_config_ops = {
  "ocamlyices.ctx_config",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

struct custom_operations ocamlyices_param_ops = {
  "ocamlyices.param",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

struct custom_operations ocamlyices_model_ops = {
  "ocamlyices.model",
  custom_finalize_default,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

value term_option(term_t t) {
  CAMLparam0();
  CAMLlocal1(res);
  if (t == NULL_TERM) {
    res = Val_int(0);
  } else {
    res = caml_alloc(1, 0);
    Store_field(res, 0, Val_term(t));
  }
  CAMLreturn(res);
}

value type_option(type_t t) {
  CAMLparam0();
  CAMLlocal1(res);
  if (t == NULL_TYPE) {
    res = Val_int(0);
  } else {
    res = caml_alloc(1, 0);
    Store_field(res, 0, Val_type(t));
  }
  CAMLreturn(res);
}

value option_some(value v) {
  CAMLparam0();
  CAMLlocal1(res);
  res = caml_alloc(1, 0);
  Store_field(res, 0, v);
  CAMLreturn(res);
}

value option_none() {
  return Val_int(0);
}

char error_message[256];

void ocamlyices_check_failure() {
  CAMLparam0();
  CAMLlocal2(arg1,arg2);
  value args[2];
  error_code_t ec = yices_error_code();
  if (ec == NO_ERROR) return;
  error_report_t * report = yices_error_report();
  snprintf(error_message, 256, "Yices Error #%d", ec);
  arg1 = caml_copy_string(error_message);
  arg2 = caml_alloc_tuple(8);
  Store_field(arg2, 0, Val_int(ec));
  Store_field(arg2, 1, Val_int(report->column));
  Store_field(arg2, 2, Val_int(report->line));
  Store_field(arg2, 3, term_option(report->term1));
  Store_field(arg2, 4, type_option(report->type1));
  Store_field(arg2, 5, term_option(report->term2));
  Store_field(arg2, 6, type_option(report->type2));
  Store_field(arg2, 7, caml_copy_int64(report->badval));
  args[0] = arg1;
  args[1] = arg2;
  yices_clear_error();
  caml_raise_with_args(*caml_named_value("ocamlyices.exception"), 2, args);
  caml_failwith(error_message);
  CAMLreturn0;
}

void ocamlyices_failure() {
  ocamlyices_check_failure();
  caml_failwith("Unknown error");
}

void ocamlyices_eval_binding_overflow() {
  caml_failwith("Term evaluation overflow Ocaml type");
}
void ocamlyices_already_freed_model() {
  caml_failwith("Illegal operation on freed model");
}
void ocamlyices_already_freed_config() {
  caml_failwith("Illegal operation on freed config");
}
void ocamlyices_already_freed_context() {
  caml_failwith("Illegal operation on freed context");
}
void ocamlyices_allocation_error() {
  caml_failwith("Illegal operation on freed model");
}
void ocamlyices_bad_array_sizes_error() {
  caml_invalid_argument("Arrays with different sizes");
}
