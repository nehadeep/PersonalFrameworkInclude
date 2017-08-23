var suggesturl = ""; // Global link to the server-side script, that gives you the suggestion list.
                     // Used for controls that do not define their own server script urls.
// pre-load images:
//var image = new Array(4);

//image[0] = new Image(), image[1] = new Image(),
//image[2] = new Image(), image[3] = new Image();

//image[0].src = "/content/testing/menus/arrow-down.gif", image[1].src = "/content/testing/menus/arrow-down-d.gif";
//image[2].src = "/content/testing/menus/arrow-up.gif",   image[3].src = "/content/testing/menus/arrow-up-d.gif";

function actb(id, ca, url)
{
	// Public Variables:

	this.actb_suggesturl  = url || (ca ? "" : suggesturl);
											    // link to the server-side script, that gives you the suggestion list
	this.actb_timeOut     = -1;                 // autocomplete Timeout in ms (-1: autocomplete never time out)
	this.actb_response    = 200;                // time, in milliseconds, between the last char typed and the actual query
	this.actb_lim         = 10;                 // number of elements autocomplete will show
	this.actb_maxLim      = false;              // should the control limit the scroll down max to limit (true) or no limit on scroll down (false)?

	this.actb_firstText   = true;               // should the auto complete be limited to the beginning of keyword?
	this.actb_firstMatch  = false;              // if previous is false, should the exact matches be displayed first?
	this.actb_fullRefresh = false;              // should the script re-send the AJAX request after each entered character?

	this.actb_useIFrame   = true;               // should the control use an IFrame element to fix suggestion list positioning (MS IE only)?
	this.actb_useScroll   = false;              // should the control use a scroll bar (true) or a up/down buttons (false)?
	this.actb_mouse       = true;               // enable mouse support
	this.actb_noDefault   = true;               // should the control omit selecting the 1st item in a suggestion list?
	this.actb_delimiter   = new Array();        // delimiter for multiple autocomplete. Set it to empty array for single autocomplete
	this.actb_startcheck  = 2;                  // show widget only after this number of characters is typed in.

	this.actb_selectedIndex = -1;               // index (zero-based) of the element last chosen
	this.actb_useProductCount = true;           // enable the product count to be displayed in the list
	this.actb_navigatePath = '/shop/search/';   // path for the search.
	this.actb_navigatePage = 'result.aspx';    // page for the search 

	// Styles:
	this.actb_arColor   = '#656291';  // background color for the "arrows"
	this.actb_bgColor   = '#e3e2dd';
	this.actb_textColor = '#333';
	this.actb_hColor    = '#fff';
	this.actb_fFamily   = 'verdana, helvetica, arial, sans-serif';
	this.actb_arrowSize = '7px';
	this.actb_fSize     = '10px';
	this.actb_hStyle    = 'font-family:verdana, helvetica, arial, sans-serif;';

	// "Private" Variables:
	this.actb_delimwords = [];
	this.actb_cdelimword = 0;
	this.actb_delimchar  = [];
	this.actb_display    = false;

	this.actb_pos    = 0;
	this.actb_total  = 0;
	this.actb_rangeu = 0;
	this.actb_ranged = 0;
	this.actb_bool   = [];
	this.actb_pre    = 0;
	this.actb_toid   = 0;
	this.actb_tomake = false;

	this.cur_x = 0;
	this.cur_y = 0;
	this.cur_w = 0;
	this.cur_h = 0;

	this.actb_mouse_on_list = 0;
	this.actb_caretmove     = false;

	this.actb_base_id  = id;
	this.actb_curr     = document.getElementById(id);
	this.actb_prevterm = this.actb_curr.value;

	this.actb_keywords = [];
	this.actb_values   = [];
	this.actb_count   = [];

	this.actb_keywords_init = [];
	this.actb_values_init   = [];
	this.actb_count_init   = [];

	ca = ca || [];
	for(var i = 0, cl = ca.length; i < cl; i++)
	{
		if(String(typeof(ca[i])).toLowerCase() == "string")
		{
			this.actb_keywords[i] = this.actb_keywords_init[i] = ca[i];
			this.actb_values[i]   = this.actb_values_init[i]   = "";
			this.actb_count[i]   = this.actb_count_init[i]   = "";
		}
		else
		{
			this.actb_keywords[i] = this.actb_keywords_init[i] = ca[i][0];
			this.actb_values[i]   = this.actb_values_init[i]   = ca[i][1];
			if (this.actb_useProductCount)
			    this.actb_count[i]   = this.actb_count_init[i]   = ca[i][2];
			else
			    this.actb_count[i]   = this.actb_count_init[i]   = "";
		}
	}

	return this.construct();
};

actb.prototype = {
    callLater: function(func, obj)
    { return function() { func.call(obj) }; },

    construct: function() {
        this.actb_curr.actb = this;

        // pre-create event functions
        this.funcClick = this.actb_mouseclick;
        this.funcCheck = this.actb_checkkey;

        this.funcHighlight = this.actb_table_highlight;

        this.funcClear = this.callLater(this.actb_clear, this);
        this.funcPress = this.callLater(this.actb_keypress, this);

        this.funcUp = this.callLater(this.actb_goup, this);
        this.funcDown = this.callLater(this.actb_godown, this);

        this.funcFocus = this.callLater(this.actb_table_focus, this);
        this.funcUnfocus = this.callLater(this.actb_table_unfocus, this);

        addEvent(this.actb_curr, "focus", this.callLater(this.actb_setup, this));

        return this;
    },

    actb_setup: function() {
        //To clear defaultSearchText
        if (this.actb_curr.value == 'Search for...') { this.actb_curr.value = '' };
        addEvent(document, "keydown", this.funcCheck);
        addEvent(this.actb_curr, "blur", this.funcClear);
        addEvent(document, "keypress", this.funcPress);
    },

    actb_clear: function() {
        var msie = (document.all && !window.opera) ? true : false;
        var event = window.event;

        //To restore defaultSearchText
        if (this.actb_curr.value == '') { this.actb_curr.value = 'Search for...' };

        if (msie && event && this.cur_h) {
            var x = event.x, y = event.y;

            if (((x > this.cur_x) && (x < (this.cur_x + this.cur_w))) && ((y > this.cur_y) && (y < (this.cur_y + this.cur_h)))) {
                this.actb_curr.focus();
                return;
            }
        }

        removeEvent(document, "keydown", this.funcCheck);
        removeEvent(this.actb_curr, "blur", this.funcClear);
        removeEvent(document, "keypress", this.funcPress);

        this.actb_removedisp();
    },

    actb_parse: function(n) {
        if (!n || !n.length) return n;

        var t, plen;
        //		if(this.actb_delimiter.length > 0)
        //		{
        //			   t = this.actb_delimwords[this.actb_cdelimword].trim().addslashes();
        //			plen = this.actb_delimwords[this.actb_cdelimword].trim().length;
        //		}
        //		else
        //		{
        t = this.actb_curr.value.addslashes();
        plen = this.actb_curr.value.length;
        //		}

        if (!plen) return n;

        var tobuild = '';

        var re = this.actb_firstText ? new RegExp("^" + t, "i") : new RegExp(t, "i");
        var p = n.search(re);

        tobuild = n.substr(0, p);

        tobuild += "<b><font style='" + (this.actb_hStyle) + "'>";

        tobuild += n.substring(p, plen + p);

        tobuild += "</font></b>";

        tobuild += n.substring(plen + p, n.length);

        return tobuild;
    },

    actb_generate: function() {
        var body = document.getElementById('tat_table_' + this.actb_base_id);
        if (body) {
            this.actb_display = false;
            document.body.removeChild(body);

            var helper = document.getElementById('tat_helper_' + this.actb_base_id);
            if (helper)
                document.body.removeChild(helper);
        }

        if (this.actb_total == 0) {
            this.actb_display = false;
            return;
        }

        var msie = (document.all && !window.opera) ? true : false;

        var bb = document.createElement('div');
        bb.id = 'tat_table_' + this.actb_base_id;
        bb.style.position = 'absolute';
        bb.style.border = '#000000 solid 1px';
        bb.style.zIndex = 3000;

        this.cur_y = eval(curTop(this.actb_curr) + this.actb_curr.offsetHeight);
        bb.style.top = this.cur_y + "px";

        this.cur_x = bb.style.left = curLeft(this.actb_curr);
        bb.style.left = this.cur_x + "px";

        this.cur_w = this.actb_curr.offsetWidth - (msie ? 0 : 2);
        bb.style.width = this.cur_w + "px";

        this.cur_h = 1;
        bb.style.height = "1px"

        var cc = null;
        if (msie && this.actb_useIFrame) {
            var cc = document.createElement('iframe');
            cc.id = "tat_helper_" + this.actb_base_id;

            cc.src = "javascript:'<html></html>';";
            cc.scrolling = "no";
            cc.frameBorder = "no";

            cc.style.display = "block";
            cc.style.position = "absolute";

            cc.style.zIndex = 99;
            cc.style.filter = "progid:DXImageTransform.Microsoft.Alpha(opacity=0)";
        }

        var actb_str = "<table cellspacing='0px' cellpadding='2px' style='width:100%;background-color:" + this.actb_bgColor + "' id='tat_table2_" + this.actb_base_id + "'>";

        if (this.actb_useScroll && (this.actb_total > this.actb_lim)) {
            this.cur_h = this.actb_lim * parseInt(this.actb_fSize);
            bb.style.height = this.cur_h + "px";

            bb.style.overflow = "auto";
            bb.style.overflowX = "hidden";
        }

        if (cc) {
            document.body.appendChild(cc);

            cc.style.top = this.cur_y + "px";
            cc.style.left = this.cur_x + "px";

            cc.style.width = bb.offsetWidth + 2;
        }
        document.body.appendChild(bb);

        var counter = 0, first = true, j = 1;

        for (var i = 0; i < this.actb_keywords.length; i++) {
            if (!this.actb_useScroll && ((this.actb_keywords.length > this.actb_lim) && (this.actb_total > this.actb_lim) && !i)) {
                //actb_str += "<tr style='background-color:" + this.actb_arColor + "'>";

                //actb_str += "<td style='color:" + this.actb_textColor + ";font-family:arial narrow;font-size:" + this.actb_arrowSize + ";cursor:default" + "' align='center'></td></tr>";
            }

            if (!i) {
                actb_str += "<tr colspan='2' style='background-color:'" + this.actb_bgColor + "'><td style='font-weight:bold;color:" + this.actb_textColor + ";font-family:" + this.actb_fFamily + ";font-size:" + this.actb_fSize + ";white-space:nowrap" + "'>" + "Matching Product Categories" + "</td></tr>";
            }

            if (this.actb_bool[i] && (this.actb_useScroll || (counter < this.actb_lim))) {
                counter++;

                actb_str += "<tr style='background-color:";

                if ((first && !this.actb_noDefault && !this.actb_tomake)) {
                    actb_str += this.actb_hColor;
                    first = false;
                }
                else {
                    actb_str += this.actb_bgColor;
                }
                actb_str += "' id='tat_tr_" + this.actb_base_id + String(j) + "'>";

                actb_str += "<td style='color:" + this.actb_textColor + ";font-family:" + this.actb_fFamily + ";font-size:" + this.actb_fSize + ";white-space:nowrap" + "' id='tat_td_" + this.actb_base_id + String(j) + "'>" + this.actb_parse(this.actb_keywords[i]) + "</td>";
                if (this.actb_useProductCount)
                    actb_str += "<td  style='color:" + this.actb_textColor + ";font-family:" + this.actb_fFamily + ";font-size:" + this.actb_fSize + ";white-space:nowrap" + "' id='tat_td_" + this.actb_base_id + String(j) + "' align='right'>(" + this.actb_count[i] + ")</td>";
                actb_str += "</tr>";

                j++;
            }
        }

        if (!this.actb_useScroll && (this.actb_total > this.actb_lim)) {
            //actb_str += "<tr style='background-color:" + this.actb_arColor + "'>";

            //actb_str += "<td style='color:" + this.actb_textColor + ";font-family:arial narrow;font-size:" + this.actb_arrowSize + ";cursor:default" + "' align='center'></td></tr>";
        }

        bb.innerHTML = actb_str;

        var table = bb.firstChild, row_num = table.rows.length, counter = 0, j = 1, real_height = 0, real_width = 0;
        if (this.actb_mouse) {
            table.onmouseout = this.funcUnfocus;
            table.onmouseover = this.funcFocus;
        }

        for (i = 0; i < row_num; i++) {
            var row = table.rows[i];
            var cell = row.cells[0];

            // did not comment out to account since the first row is always Matching product category
            // not to highlight the first row.
            if (!i) {
                //replaceHTML(cell, image[3]); 

                // Up arrow:
                //real_height += row.offsetHeight + 1;
            }
            // commented out to account since there is no image row
            //else if((i == (row_num - 1)) && (!this.actb_useScroll && (this.actb_total > this.actb_lim)))
            //{
            //replaceHTML(cell, image[0]);

            // Down arrow:
            //addEvent(cell, "click", this.funcDown);

            //real_height += row.offsetHeight + 1;
            //}
            else {
                counter++;

                // Content cells:
                cell.actb = this;
                cell.setAttribute('pos', j);

                if (counter <= this.actb_lim)
                    real_height += row.offsetHeight + 1;

                if (real_width < row.offsetWidth)
                    real_width = row.offsetWidth;

                if (this.actb_mouse) {
                    cell.style.cursor = 'pointer';
                    addEvent(cell, "click", this.funcClick);
                    cell.onmouseover = this.funcHighlight;
                }

                if (this.actb_useProductCount) {
                    var countcell = row.cells[1];

                    // Content cells:
                    countcell.actb = this;
                    countcell.setAttribute('pos', j);

                    if (this.actb_mouse) {
                        countcell.style.cursor = 'pointer';
                        addEvent(countcell, "click", this.funcClick);
                        countcell.onmouseover = this.funcHighlight;
                    }
                }
                j++;
            }
        }

        real_height += (msie ? 3 : 1);
        this.cur_h = real_height;
        bb.style.height = real_height + "px";

        this.cur_w = (real_width > bb.offsetWidth ? real_width : bb.offsetWidth) + 2;
        bb.style.width = this.cur_w + "px";

        if (cc) {
            this.cur_h = real_height;

            cc.style.height = bb.style.height = this.cur_h + "px";
            cc.style.width = this.cur_w + "px";
        }

        this.actb_pos = this.actb_noDefault ? 0 : 1;
        this.actb_rangeu = 1;
        this.actb_ranged = j - 1;
        this.actb_display = true;
    },

    actb_remake: function() {
        var a = document.getElementById('tat_table2_' + this.actb_base_id);

        if (this.actb_mouse) {
            a.onmouseout = this.funcUnfocus;
            a.onmouseover = this.funcFocus;
        }

        var i, k = 1;
        var first = true;
        var j = 1;

        //		if(this.actb_total > this.actb_lim)
        //		{
        //		    var b = (this.actb_rangeu > 1);

        //			var r = a.rows[k++];
        //			r.style.backgroundColor = this.actb_arColor;

        //			var c = r.firstChild;
        //			c.style.color = this.actb_textColor;
        //			c.style.fontFamily = 'arial narrow';
        //			c.style.fontSize = this.actb_arrowSize;
        //			c.style.cursor = 'default';
        //			c.align = 'center';

        //			replaceHTML(c, b ? image[2] : image[3]);

        //			if(b)
        //			{
        //				addEvent(c, "click", this.funcUp);
        //				c.style.cursor = 'pointer';
        //			}
        //			else
        //			{
        //				c.style.cursor = 'default';
        //			}
        //		}

        for (var i = 0; i < this.actb_keywords.length; i++) {
            if (this.actb_bool[i]) {
                if (j >= this.actb_rangeu && j <= this.actb_ranged) {
                    var r = a.rows[k++];
                    r.style.backgroundColor = this.actb_bgColor;
                    r.id = 'tat_tr_' + this.actb_base_id + String(j);

                    var c = r.childNodes[0];
                    c.style.color = this.actb_textColor;
                    c.style.fontFamily = this.actb_fFamily;
                    c.style.fontSize = this.actb_fSize;
                    c.innerHTML = this.actb_parse(this.actb_keywords[i]);
                    c.id = 'tat_td_' + this.actb_base_id + String(j);
                    c.setAttribute('pos', j);
                    if (this.actb_useProductCount) {
                        var countc = r.childNodes[1];
                        countc.style.color = this.actb_textColor;
                        countc.style.fontFamily = this.actb_fFamily;
                        countc.style.fontSize = this.actb_fSize;
                        countc.innerHTML = this.actb_count[i];
                        countc.id = 'tat_td_' + this.actb_base_id + String(j);
                        countc.setAttribute('pos', j);
                    }
                    j++;
                }
                else {
                    j++;
                }
            }

            if (j > this.actb_ranged) break;
        }

        //		if(this.actb_keywords.length > this.actb_lim)
        //		{
        //			var b = ((j - 1) < this.actb_total);

        //			var r = a.rows[k];
        //			r.style.backgroundColor = this.actb_arColor;

        //			var c = r.firstChild;
        //			c.style.color = this.actb_textColor;
        //			c.style.fontFamily = 'arial narrow';
        //			c.style.fontSize = this.actb_arrowSize;
        //			c.style.cursor = 'default';
        //			c.align = 'center';

        //			replaceHTML(c, b ? image[0] : image[1]);

        //			if(b)
        //			{
        //				addEvent(c, "click", this.funcDown);
        //				c.style.cursor = 'pointer';
        //			}
        //			else
        //			{
        //				c.style.cursor = 'default';
        //			}
        //		}

        if ((document.all && !window.opera)) {
            var helper = document.getElementById("tat_helper_" + this.actb_base_id);
            if (helper)
                helper.style.width = a.parentNode.offsetWidth + 1;
        }
    },

    actb_goup: function() {
        this.actb_curr.focus();

        if (!this.actb_display) return;
        if (this.actb_pos <= 1) return;

        var t = document.getElementById('tat_tr_' + this.actb_base_id + String(this.actb_pos));
        if (t && t.style) t.style.backgroundColor = this.actb_bgColor;

        this.actb_pos--;
        t = document.getElementById('tat_tr_' + this.actb_base_id + String(this.actb_pos));

        if (this.actb_useScroll && t) {
            var base = document.getElementById('tat_table_' + this.actb_base_id);
            base.scrollTop = t.offsetTop;
        }
        else {
            if (this.actb_pos < this.actb_rangeu) {
                this.actb_rangeu--;
                this.actb_ranged--;
                this.actb_remake();
            }

            t = document.getElementById('tat_tr_' + this.actb_base_id + String(this.actb_pos));
        }

        if (t && t.style) t.style.backgroundColor = this.actb_hColor;

        if (this.actb_toid) {
            clearTimeout(this.actb_toid);
            this.actb_toid = 0;
        }

        if (this.actb_timeOut > 0)
            this.actb_toid = setTimeout(function() { this.actb_mouse_on_list = 1; this.actb_removedisp(); }, this.actb_timeOut);

        this.actb_curr.focus();
    },

    actb_godown: function() {
        this.actb_curr.focus();

        if (!this.actb_display) return;
        if (this.actb_pos == this.actb_total) return;

        if (this.actb_pos >= 1) {
            var t = document.getElementById('tat_tr_' + this.actb_base_id + String(this.actb_pos));
            if (t && t.style) t.style.backgroundColor = this.actb_bgColor;
        }
        else {
            this.actb_pos = 0;
        }

        if ((this.actb_pos < this.actb_ranged) || !this.actb_maxLim)
            this.actb_pos++;
        t = document.getElementById('tat_tr_' + this.actb_base_id + String(this.actb_pos));

        if (this.actb_useScroll && t) {
            var base = document.getElementById('tat_table_' + this.actb_base_id);
            base.scrollTop = t.offsetTop;
        }
        else {
            if (this.actb_pos > this.actb_ranged) {
                this.actb_rangeu++;
                this.actb_ranged++;
                this.actb_remake();
            }

            t = document.getElementById('tat_tr_' + this.actb_base_id + String(this.actb_pos));
        }

        if (t && t.style) t.style.backgroundColor = this.actb_hColor;

        if (this.actb_toid) {
            clearTimeout(this.actb_toid);
            this.actb_toid = 0;
        }

        if (this.actb_timeOut > 0)
            this.actb_toid = setTimeout(function() { this.actb_mouse_on_list = 1; this.actb_removedisp(); }, this.actb_timeOut);

        this.actb_curr.focus();
    },

    actb_mouseclick: function(event) {
        var elem = getTargetElement(event);
        if (!elem.id) elem = elem.parentNode;

        var obj = elem.actb;
        if (!obj) {
            elem = elem.parentNode;
            obj = elem.actb;
        }
        if (!obj || !obj.actb_display) return;

        obj.actb_mouse_on_list = 0;
        obj.actb_pos = elem.getAttribute('pos');
        obj.actb_penter();
        //searchform.submit();
    },

    actb_table_focus: function()
    { this.actb_mouse_on_list = 1; },

    actb_table_unfocus: function() {
        this.actb_mouse_on_list = 0;

        if (this.actb_toid) {
            clearTimeout(this.actb_toid);
            this.actb_toid = 0;
        }

        if (this.actb_timeOut > 0)
            this.actb_toid = setTimeout(function() { obj.actb_mouse_on_list = 0; this.actb_removedisp(); }, this.actb_timeOut);
    },

    actb_table_highlight: function(event) {
        var elem = getTargetElement(event);

        var obj = elem.actb;
        if (!obj) return;

        obj.actb_mouse_on_list = 1;

        var row = document.getElementById('tat_tr_' + obj.actb_base_id + obj.actb_pos);
        if (row && row.style) row.style.backgroundColor = obj.actb_bgColor;

        obj.actb_pos = elem.getAttribute('pos');

        row = document.getElementById('tat_tr_' + obj.actb_base_id + obj.actb_pos);
        if (row && row.style)
            row.style.backgroundColor = obj.actb_hColor;

        if (obj.actb_toid) {
            clearTimeout(obj.actb_toid);
            obj.actb_toid = 0;
        }

        if (obj.actb_timeOut > 0)
            obj.actb_toid = setTimeout(function() { obj.actb_mouse_on_list = 0; obj.actb_removedisp(); }, obj.actb_timeOut);
    },

    actb_penter: function() {
        if (!this.actb_display) return;
        if (this.actb_pos < 1) {
            searchform.submit();
            return;
        }

        this.actb_selectedIndex = this.actb_pos - 1;
        this.actb_display = false;

        var webClass = '', keyword = '', c = 0;

        for (var i = 0; i < this.actb_keywords.length; i++) {
            if (this.actb_bool[i]) c++;

            if (c == this.actb_pos) {
                webClass = this.actb_values[i];
                keyword = this.actb_keywords[i];
                break;
            }
        }

        // Changed - not to insert the word but rather redirec to a URL.
        //this.actb_insertword(word);
        window.location.href = encodeURI(this.actb_navigatePath + keyword.replace(/[-& \/,()]+/g, '-').replace(/\.+$/g, '')
            + '/' + this.actb_navigatePage + '?w=' + webClass);
    },

    actb_insertword: function(a) {
        //		if(this.actb_delimiter.length > 0)
        //		{
        //			var str = '';

        //			for(var i = 0; i < this.actb_delimwords.length; i++)
        //			{
        //				if(this.actb_cdelimword == i)
        //				{
        //					prespace = postspace = '';
        //					gotbreak = false;
        //					for(var j = 0; j < this.actb_delimwords[i].length; ++j)
        //					{
        //						if(this.actb_delimwords[i].charAt(j) != ' ')
        //						{
        //							gotbreak = true;
        //							break;
        //						}

        //						prespace += ' ';
        //					}

        //					for(j = this.actb_delimwords[i].length - 1; j >= 0; --j)
        //					{
        //						if(this.actb_delimwords[i].charAt(j) != ' ') break;
        //						postspace += ' ';
        //					}

        //					str += prespace;
        //					str += a;
        //					if(gotbreak) str += postspace;
        //				}
        //				else
        //				{
        //					str += this.actb_delimwords[i];
        //				}

        //				if(i != this.actb_delimwords.length - 1)
        //					str += this.actb_delimchar[i];
        //			}

        //			this.actb_curr.value = str;
        //			setCaret(this.actb_curr, this.actb_curr.value.length);
        //		}
        //		else
        //		{
        this.actb_curr.value = a;
        //		}

        this.actb_mouse_on_list = 0;
        this.actb_removedisp();
    },

    actb_removedisp: function() {
        if (this.actb_mouse_on_list == 0) {
            this.actb_display = 0;

            var base = document.getElementById('tat_table_' + this.actb_base_id);
            if (base) {
                var helper = document.getElementById('tat_helper_' + this.actb_base_id);
                if (helper)
                    document.body.removeChild(helper);

                document.body.removeChild(base);
            }

            if (this.actb_toid) {
                clearTimeout(this.actb_toid);
                this.actb_toid = 0;
            }

            this.cur_x = 0;
            this.cur_y = 0;
            this.cur_w = 0;
            this.cur_h = 0;
        }
    },

    actb_keypress: function(event) {
        if (this.actb_caretmove) stopEvent(event);
        return !this.actb_caretmove;
    },

    actb_checkkey: function(event) {
        event = event || window.event;

        var code = event.keyCode;
        var obj = getTargetElement(event).actb;
        obj.actb_caretmove = 0;

        var term = "";

        if (obj.actb_toid) {
            clearTimeout(obj.actb_toid);
            obj.actb_toid = 0;
        }

        switch (code) {
            // Up arrow: 
            case 38:
                obj.actb_goup();
                obj.actb_caretmove = 1;
                return false;
                break;

            // Down arrow: 
            case 40:
                if (!obj.actb_display) {
                    obj.actb_toid = setTimeout(function() {
                        obj.actb_tocomplete.call(obj, -1);
                    },
					25);
                }
                else {
                    obj.actb_godown();
                    obj.actb_caretmove = 1;
                }
                return false;
                break;

            // Page up: 
            case 33:
                for (var c = 0; c < obj.actb_lim; c++)
                    obj.actb_goup();

                obj.actb_caretmove = 1;
                break;

            // Page down: 
            case 34:
                for (var c = 0; c < obj.actb_lim; c++)
                    obj.actb_godown();

                obj.actb_caretmove = 1;
                break;

            // Esc: 
            case 27:
                term = obj.actb_curr.value;

                obj.actb_mouse_on_list = 0;
                obj.actb_removedisp();
                break;

            // Enter: 
            case 13:
                if (obj.actb_display) {
                    obj.actb_caretmove = 1;
                    obj.actb_penter();
                    //searchform.submit();
                    return false;
                }
                //else
                //{
                //	searchform.submit();
                //}
                break;

            // Tab: 
            case 9:
                if ((obj.actb_display && obj.actb_pos) || obj.actb_toid) {
                    obj.actb_caretmove = 1;
                    obj.actb_penter();

                    setTimeout(function() { obj.actb_curr.focus(); }, 25);
                    return false;
                }
                break;

            default:
                obj.actb_caretmove = 0;
                obj.actb_toid = setTimeout(function() {
                    obj.actb_tocomplete.call(obj, code);
                },
				(obj.actb_response < 10 ? 10 : obj.actb_response));
                break;
        }

        if (term.length) setTimeout(function() { obj.actb_curr.value = term; }, 25);
        return true;
    },

    actb_tocomplete: function(kc) {
        if (this.actb_toid) {
            clearTimeout(this.actb_toid);
            this.actb_toid = 0;
        }
        else {
            return;
        }

        if (this.actb_display && (this.actb_prevterm == this.actb_curr.value)) return;
        this.actb_prevterm = this.actb_curr.value;

        if (kc == 38 || kc == 40 || kc == 13) return;

        if (this.actb_display) {
            var word = 0;
            var c = 0;

            for (var i = 0; i <= this.actb_keywords.length; i++) {
                if (this.actb_bool[i]) c++;

                if (c == this.actb_pos) {
                    word = i;
                    break;
                }
            }

            this.actb_pre = word;
        }
        else {
            this.actb_pre = -1;
        }

        if (!this.actb_curr.value.length && (kc != -1)) {
            this.actb_mouse_on_list = 0;
            this.actb_removedisp();
        }

        var ot, t;

        //		if(this.actb_delimiter.length > 0)
        //		{
        //			var caret_pos_end = this.actb_curr.value.length;

        //			var delim_split = '';
        //			for(var i = 0; i < this.actb_delimiter.length; i++)
        //				delim_split += this.actb_delimiter[i];

        //		    delim_split = delim_split.addslashes();
        //			var delim_split_rx = new RegExp("([" + delim_split + "])");
        //			c = 0;
        //			this.actb_delimwords = [];
        //			this.actb_delimwords[0] = '';

        //			for(var i = 0, j = this.actb_curr.value.length; i < this.actb_curr.value.length; i++, j--)
        //			{
        //				if(this.actb_curr.value.substr(i, j).search(delim_split_rx) == 0)
        //				{
        //					ma = this.actb_curr.value.substr(i,j).match(delim_split_rx);
        //					this.actb_delimchar[c] = ma[1];
        //					c++;
        //					this.actb_delimwords[c] = '';
        //				}
        //				else
        //				{
        //					this.actb_delimwords[c] += this.actb_curr.value.charAt(i);
        //				}
        //			}

        //			var l = 0;
        //			this.actb_cdelimword = -1;
        //			for(i = 0; i < this.actb_delimwords.length; i++)
        //			{
        //				if((caret_pos_end >= l) && (caret_pos_end <= l + this.actb_delimwords[i].length))
        //					this.actb_cdelimword = i;

        //				l += this.actb_delimwords[i].length + 1;
        //			}

        //			ot = this.actb_delimwords[this.actb_cdelimword].trim(); 
        //			 t = this.actb_delimwords[this.actb_cdelimword].addslashes().trim();
        //		}
        //		else
        //		{
        ot = this.actb_curr.value;
        t = this.actb_curr.value.addslashes();
        //		}
        if (ot.length == 0 && (kc != -1)) {
            this.actb_mouse_on_list = 0;
            this.actb_removedisp();
        }
        else if ((ot.length == this.actb_startcheck) || this.actb_fullRefresh ||
		       ((ot.length > this.actb_startcheck) && !this.actb_keywords.length) ||
		       ((ot.length > this.actb_startcheck) && (this.actb_keywords[0].substr(0, ot.length).toLowerCase() != ot.toLowerCase()))) {
            if (this.actb_suggesturl.length) {
                // create xmlhttprequest object:
                var http = null;
                if (typeof XMLHttpRequest != 'undefined') {
                    try {
                        http = new XMLHttpRequest();
                    }
                    catch (e) { http = null; }
                }
                else {
                    try {
                        http = new ActiveXObject("Msxml2.XMLHTTP");
                    }
                    catch (e) {
                        try {
                            http = new ActiveXObject("Microsoft.XMLHTTP");
                        }
                        catch (e) { http = null; }
                    }
                }

                if (http) {
                    // For local debugging in Mozilla/Firefox only!
                    try {
                        netscape.security.PrivilegeManager.enablePrivilege("UniversalBrowserRead");
                    } catch (e) { }

                    if (http.overrideMimeType)
                        http.overrideMimeType('text/xml');

                    http.open("GET", this.actb_suggesturl + "key=" + ot, true);

                    var obj = this;
                    http.onreadystatechange = function(n) {
                        if (http.readyState == 4) {
                            if ((http.status == 200) || (http.status == 0)) {
                                var xmlDocument = null, tmpinfo = null;
                                var msie = (document.all && !window.opera) ? true : false;

                                try {
                                    xmlDocument = http.responseXML;
                                    if (!msie)
                                        xmlDocument.normalize();
                                    tmpinfo = xmlDocument.getElementsByTagName("listdata").item(0).firstChild.data;
                                }
                                catch (e) {
                                    try {
                                        xmlDocument = (new DOMParser()).parseFromString(http.responseText, "text/xml");
                                        tmpinfo = xmlDocument.getElementsByTagName("listdata").item(0).firstChild.data;
                                    }
                                    catch (ee) { }
                                }

                                if (tmpinfo) {
                                    obj.actb_keywords = tmpinfo.split('|');
                                    var keyword_number = obj.actb_keywords.length;
                                    for (var i = 0; i < keyword_number; i++) {
                                        var ca = obj.actb_keywords[i], comma = ca.indexOf(":");

                                        if (comma != -1) {
                                            var ci = ca.split(':');

                                            obj.actb_keywords[i] = obj.actb_keywords_init[i] = ci[0];
                                            obj.actb_values[i] = obj.actb_values_init[i] = ci[1];
                                            if (obj.actb_useProductCount)
                                                obj.actb_count[i] = obj.actb_count_init[i] = ci[2];
                                            else
                                                obj.actb_count[i] = obj.actb_count_init[i] = "";
                                        }
                                        else {
                                            obj.actb_values[i] = obj.actb_values_init[i] = "";
                                            obj.actb_count[i] = obj.actb_count_init[i] = "";
                                        }
                                    }

                                    obj.done.call(obj, ot, t);
                                }
                                // hide popup if no matched found
                                else {
                                    obj.actb_removedisp();
                                }
                            }
                        }
                    }

                    http.send(null);
                }

                // xmlhttp object creation failed
                return;
            }
            else {
                this.done(ot, t);
            }
        }
        else {
            this.done(ot, t);
        }
    },

    done: function(ot, t) {
        if (ot.length < this.actb_startcheck) {
            this.actb_removedisp();
            return;
        }

        var re = new RegExp(((!this.actb_firstText && !this.actb_firstMatch) ? "" : "^") + t, "i");

        this.actb_total = 0;
        this.actb_tomake = false;

        var al = this.actb_keywords.length;

        for (var i = 0; i < al; i++) {
            this.actb_bool[i] = false;
            if (re.test(this.actb_keywords[i])) {
                this.actb_total++;
                this.actb_bool[i] = true;

                if (this.actb_pre == i) this.actb_tomake = true;
            }
        }

        if (!this.actb_curr.value.length) {
            for (i = 0; i < al; i++) {
                this.actb_keywords[i] = this.actb_keywords_init[i];
                this.actb_values[i] = this.actb_values_init[i];
                if (this.actb_useProductCount)
                    this.actb_count[i] = this.actb_count_init[i];
                this.actb_bool[i] = true;
            }
        }
        else if (!this.actb_firstText && this.actb_firstMatch) {
            var tmp = [], tmpv = [];

            for (i = 0; i < al; i++) {
                if (this.actb_bool[i]) {
                    tmp[tmp.length] = this.actb_keywords[i];
                    tmpv[tmpv.length] = this.actb_values[i];
                }
            }

            re = new RegExp(t, "i");

            for (i = 0; i < al; i++) {
                if (re.test(this.actb_keywords[i]) && !this.actb_bool[i]) {
                    this.actb_total++;
                    this.actb_bool[i] = true;

                    if (this.actb_pre == i) this.actb_tomake = true;

                    tmp[tmp.length] = this.actb_keywords[i];
                    tmpv[tmpv.length] = this.actb_values[i];
                }
            }

            for (i = 0; i < al; i++) {
                if (!this.actb_bool[i]) {
                    tmp[tmp.length] = this.actb_keywords[i];
                    tmpv[tmpv.length] = this.actb_values[i];
                }
            }

            for (i = 0; i < al; i++) {
                this.actb_keywords[i] = tmp[i];
                this.actb_values[i] = tmpv[i];
            }

            for (i = 0; i < al; i++)
                this.actb_bool[i] = (i < this.actb_total) ? true : false;
        }

        if (this.actb_timeOut > 0)
            this.actb_toid = setTimeout(function() { this.actb_mouse_on_list = 0; this.actb_removedisp(); }, this.actb_timeOut);

        this.actb_generate();
    }
}

// Supplementary functions

// Add an event to the obj given
// event_name refers to the event trigger, without the "on", like click or mouseover
// func_name refers to the function callback when event is triggered
function addEvent(obj, event_name, func_ref)
{
    // Added firefox since the enter key does not work with listeners
	if(obj.addEventListener && !window.opera && (navigator.userAgent.toLowerCase().indexOf('firefox')==-1))
	{
		obj.addEventListener(event_name, func_ref, true);
	}
	else
	{
		obj["on" + event_name] = func_ref;
	}
}

// Removes an event from the object
function removeEvent(obj, event_name, func_ref)
{
    // Added firefox since the enter key does not work with listeners
	if(obj.removeEventListener && !window.opera && (navigator.userAgent.toLowerCase().indexOf('firefox')==-1))
	{
		obj.removeEventListener(event_name, func_ref, true);
	}
	else
	{
		obj["on" + event_name] = null;
	}
}

// Stop an event from bubbling up the event DOM
function stopEvent(event)
{
	event = event || window.event;

	if(event)
	{
		if(event.stopPropagation) event.stopPropagation();
		if(event.preventDefault) event.preventDefault();

		if(typeof event.cancelBubble != "undefined")
		{
			event.cancelBubble = true;
			event.returnValue = false;
		}
	}

	return false;
}

// Get the obj that triggers off the event
function getTargetElement(event)
{
	event = event || window.event;
	return event.srcElement || event.target;
}
   
// Sets the caret position to l in the object
function setCaret(obj, l)
{
	obj.focus();

	if(obj.setSelectionRange)
	{
		obj.setSelectionRange(l, l);
	}
	else if(obj.createTextRange)
	{
		m = obj.createTextRange();		
		m.moveStart('character', l);
		m.collapse();
		m.select();
	}
}

// String functions
String.prototype.addslashes = function() { return this.replace(/(["\\\.\|\[\]\^\*\+\?\$\(\)])/g, '\\$1'); }

String.prototype.trim = function () { return this.replace(/^\s*(\S*(\s+\S+)*)\s*$/, "$1"); };

// Offset position from top of the screen
function curTop(obj)
{
	var toreturn = 0;
	while(obj)
	{
		toreturn += obj.offsetTop ;
		obj = obj.offsetParent;
	}

	return toreturn;
}

// Offset position from left of the screen
function curLeft(obj)
{
	var toreturn = 0;
	while(obj)
	{
		toreturn += obj.offsetLeft ;
		obj = obj.offsetParent;
	}

	return toreturn;
}

// Image installation
//function replaceHTML(obj, oImg)
//{
//	var el = obj.childNodes[0];
//	while(el)
//	{
//		obj.removeChild(el);
//		el = obj.childNodes[0];
//	}

//	obj.appendChild(oImg);
//}
