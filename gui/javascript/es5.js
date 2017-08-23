Object.getPrototypeOf || (Object.getPrototypeOf = function (t) { if (t !== Object(t)) throw TypeError("Object.getPrototypeOf called on non-object"); return t.__proto__ || t.constructor.prototype || Object.prototype }), "function" != typeof Object.getOwnPropertyNames && (Object.getOwnPropertyNames = function (t) { if (t !== Object(t)) throw TypeError("Object.getOwnPropertyNames called on non-object"); var r, e = []; for (r in t) Object.prototype.hasOwnProperty.call(t, r) && e.push(r); return e }), "function" != typeof Object.create && (Object.create = function (t, r) { function e() { } if ("object" != typeof t) throw TypeError(); e.prototype = t; var o = new e; if (t && (o.constructor = e), void 0 !== r) { if (r !== Object(r)) throw TypeError(); Object.defineProperties(o, r) } return o }), function () { if (!Object.defineProperty || !function () { try { return Object.defineProperty({}, "x", {}), !0 } catch (t) { return !1 } }()) { var t = Object.defineProperty; Object.defineProperty = function (r, e, o) { if (t) try { return t(r, e, o) } catch (n) { } if (r !== Object(r)) throw TypeError("Object.defineProperty called on non-object"); return Object.prototype.__defineGetter__ && "get" in o && Object.prototype.__defineGetter__.call(r, e, o.get), Object.prototype.__defineSetter__ && "set" in o && Object.prototype.__defineSetter__.call(r, e, o.set), "value" in o && (r[e] = o.value), r } } }(), "function" != typeof Object.defineProperties && (Object.defineProperties = function (t, r) { if (t !== Object(t)) throw TypeError("Object.defineProperties called on non-object"); var e; for (e in r) Object.prototype.hasOwnProperty.call(r, e) && Object.defineProperty(t, e, r[e]); return t }), Object.keys || (Object.keys = function (t) { if (t !== Object(t)) throw TypeError("Object.keys called on non-object"); var r, e = []; for (r in t) Object.prototype.hasOwnProperty.call(t, r) && e.push(r); return e }), Function.prototype.bind || (Function.prototype.bind = function (t) { function r() { } if ("function" != typeof this) throw TypeError("Bind must be called on a function"); var e = [].slice, o = e.call(arguments, 1), n = this, i = function () { return n.apply(this instanceof r ? this : t, o.concat(e.call(arguments))) }; return r.prototype = n.prototype, i.prototype = new r, i }), Array.isArray = Array.isArray || function (t) { return Boolean(t && "[object Array]" === Object.prototype.toString.call(Object(t))) }, Array.prototype.indexOf || (Array.prototype.indexOf = function (t) { if (void 0 === this || null === this) throw TypeError(); var r = Object(this), e = r.length >>> 0; if (0 === e) return -1; var o = 0; if (arguments.length > 0 && (o = Number(arguments[1]), isNaN(o) ? o = 0 : 0 !== o && o !== 1 / 0 && o !== -(1 / 0) && (o = (o > 0 || -1) * Math.floor(Math.abs(o)))), o >= e) return -1; for (var n = o >= 0 ? o : Math.max(e - Math.abs(o), 0) ; e > n; n++) if (n in r && r[n] === t) return n; return -1 }), Array.prototype.lastIndexOf || (Array.prototype.lastIndexOf = function (t) { if (void 0 === this || null === this) throw TypeError(); var r = Object(this), e = r.length >>> 0; if (0 === e) return -1; var o = e; arguments.length > 1 && (o = Number(arguments[1]), o !== o ? o = 0 : 0 !== o && o !== 1 / 0 && o !== -(1 / 0) && (o = (o > 0 || -1) * Math.floor(Math.abs(o)))); for (var n = o >= 0 ? Math.min(o, e - 1) : e - Math.abs(o) ; n >= 0; n--) if (n in r && r[n] === t) return n; return -1 }), Array.prototype.every || (Array.prototype.every = function (t) { if (void 0 === this || null === this) throw TypeError(); var r = Object(this), e = r.length >>> 0; if ("function" != typeof t) throw TypeError(); var o, n = arguments[1]; for (o = 0; e > o; o++) if (o in r && !t.call(n, r[o], o, r)) return !1; return !0 }), Array.prototype.some || (Array.prototype.some = function (t) { if (void 0 === this || null === this) throw TypeError(); var r = Object(this), e = r.length >>> 0; if ("function" != typeof t) throw TypeError(); var o, n = arguments[1]; for (o = 0; e > o; o++) if (o in r && t.call(n, r[o], o, r)) return !0; return !1 }), Array.prototype.forEach || (Array.prototype.forEach = function (t) { if (void 0 === this || null === this) throw TypeError(); var r = Object(this), e = r.length >>> 0; if ("function" != typeof t) throw TypeError(); var o, n = arguments[1]; for (o = 0; e > o; o++) o in r && t.call(n, r[o], o, r) }), Array.prototype.map || (Array.prototype.map = function (t) { if (void 0 === this || null === this) throw TypeError(); var r = Object(this), e = r.length >>> 0; if ("function" != typeof t) throw TypeError(); var o = []; o.length = e; var n, i = arguments[1]; for (n = 0; e > n; n++) n in r && (o[n] = t.call(i, r[n], n, r)); return o }), Array.prototype.filter || (Array.prototype.filter = function (t) { if (void 0 === this || null === this) throw TypeError(); var r = Object(this), e = r.length >>> 0; if ("function" != typeof t) throw TypeError(); var o, n = [], i = arguments[1]; for (o = 0; e > o; o++) if (o in r) { var p = r[o]; t.call(i, p, o, r) && n.push(p) } return n }), Array.prototype.reduce || (Array.prototype.reduce = function (t) { if (void 0 === this || null === this) throw TypeError(); var r = Object(this), e = r.length >>> 0; if ("function" != typeof t) throw TypeError(); if (0 === e && 1 === arguments.length) throw TypeError(); var o, n = 0; if (arguments.length >= 2) o = arguments[1]; else for (; ;) { if (n in r) { o = r[n++]; break } if (++n >= e) throw TypeError() } for (; e > n;) n in r && (o = t.call(void 0, o, r[n], n, r)), n++; return o }), Array.prototype.reduceRight || (Array.prototype.reduceRight = function (t) { if (void 0 === this || null === this) throw TypeError(); var r = Object(this), e = r.length >>> 0; if ("function" != typeof t) throw TypeError(); if (0 === e && 1 === arguments.length) throw TypeError(); var o, n = e - 1; if (arguments.length >= 2) o = arguments[1]; else for (; ;) { if (n in this) { o = this[n--]; break } if (--n < 0) throw TypeError() } for (; n >= 0;) n in r && (o = t.call(void 0, o, r[n], n, r)), n--; return o }), String.prototype.trim || (String.prototype.trim = function () { return String(this).replace(/^\s+/, "").replace(/\s+$/, "") }), Date.now || (Date.now = function () { return Number(new Date) }), Date.prototype.toISOString || (Date.prototype.toISOString = function () { function t(t) { return ("00" + t).slice(-2) } function r(t) { return ("000" + t).slice(-3) } return this.getUTCFullYear() + "-" + t(this.getUTCMonth() + 1) + "-" + t(this.getUTCDate()) + "T" + t(this.getUTCHours()) + ":" + t(this.getUTCMinutes()) + ":" + t(this.getUTCSeconds()) + "." + r(this.getUTCMilliseconds()) + "Z" });