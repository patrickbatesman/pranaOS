display/i $pc

define xbt
    set $tid=$arg0
    set $save_pc = $pc
    set $save_sp = $sp
    select-frame 0
    set $pc = 'github.com/pranaOS/pranaOS/kernel.threads'[$tid].tf.IP
    set $sp = 'github.com/pranaOS/pranaOS/kernel.threads'[$tid].tf.SP
    bt

    set $pc = $save_pc
    set $sp = $save_sp
end

define xps
    set $i = 0
    while $i < 20
        set $t = 'github.com/pranaOS/pranaOS/kernel.threads'[$i]
        set $addr = 0
        if $t.tf != 0
            set $addr = $t.tf.IP
        end
        printf "thread[%d]={state=%d, counter=%d, pc=0x%x lock=0x%x}", $i, $t.state, $t.counter, $addr, $t.sleepKey
        if $t.sleepKey != 0
            x $t.sleepKey
        end
        set $i = $i+1
    end
end